#define NTDDI_VERSION 0x06010000

#include "nan.h"

#include <Windows.h>
#include <sal.h>
#include <Psapi.h>
#include <strsafe.h>
#include <ObjBase.h>
#include <ShObjIdl.h>
#include <propvarutil.h>
#include <functiondiscoverykeys.h>
#include <intsafe.h>
#include <guiddef.h>

#include <roapi.h>
#include <wrl\client.h>
#include <wrl\implements.h>

using namespace v8;
using namespace Microsoft::WRL;

// The Code of the two functions below is a slightly modified versions of the examples from: http://msdn.microsoft.com/en-us/library/windows/desktop/hh802762

// Install the shortcut
HRESULT InstallShortcut(_In_z_ wchar_t *shortcutPath, Handle<Value> exePath, _In_z_ Handle<Value> appId)
{
  ComPtr<IShellLinkW> shellLink;
  HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellLink));

  if (SUCCEEDED(hr))
  {
    String::Value exePathValue(exePath);
    hr = shellLink->SetPath((LPCWSTR)*exePathValue);
    if (SUCCEEDED(hr))
    {
      hr = shellLink->SetArguments(L"");
      if (SUCCEEDED(hr))
      {
        ComPtr<IPropertyStore> propertyStore;

        hr = shellLink.As(&propertyStore);
        if (SUCCEEDED(hr))
        {
          PROPVARIANT appIdPropVar;
          String::Value appIdValue(appId);
          hr = InitPropVariantFromString((PCWSTR) *appIdValue, &appIdPropVar);
          if (SUCCEEDED(hr))
          {
            hr = propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar);
            if (SUCCEEDED(hr))
            {
              hr = propertyStore->Commit();
              if (SUCCEEDED(hr))
              {
                ComPtr<IPersistFile> persistFile;
                hr = shellLink.As(&persistFile);
                if (SUCCEEDED(hr))
                {
                  hr = persistFile->Save(shortcutPath, TRUE);
                }
              }
            }
            PropVariantClear(&appIdPropVar);
          }
        }
      }
    }
  }
  return hr;
}

HRESULT TryCreateShortcut(Handle<Value> exePath, Handle<Value> shortcutName, Handle<Value> appId)
{
  wchar_t shortcutPath[MAX_PATH];
  DWORD charWritten = GetEnvironmentVariableW(L"APPDATA", shortcutPath, MAX_PATH);
  HRESULT hr = charWritten > 0 ? S_OK : E_INVALIDARG;

  if (SUCCEEDED(hr))
  {
    errno_t concatError = wcscat_s(shortcutPath, ARRAYSIZE(shortcutPath), L"\\Microsoft\\Windows\\Start Menu\\Programs\\");
    hr = concatError == 0 ? S_OK : E_INVALIDARG;
    if (!SUCCEEDED(hr))
      return hr;

    String::Value shorcutNameValue(shortcutName);
    concatError = wcscat_s(shortcutPath, ARRAYSIZE(shortcutPath), reinterpret_cast<wchar_t*>(*shorcutNameValue));
    hr = concatError == 0 ? S_OK : E_INVALIDARG;
    if (!SUCCEEDED(hr))
      return hr;

    concatError = wcscat_s(shortcutPath, ARRAYSIZE(shortcutPath), L".lnk");
    hr = concatError == 0 ? S_OK : E_INVALIDARG;

    if (SUCCEEDED(hr))
    {
      DWORD attributes = GetFileAttributesW(shortcutPath);
      bool fileExists = attributes < 0xFFFFFFF;

      if (!fileExists)
      {
        hr = InstallShortcut(shortcutPath, exePath, appId);
      }
      else
      {
        hr = S_FALSE;
      }
    }
  }
  return hr;
}

NAN_METHOD(CreateShortcut)
{
  Nan::HandleScope scope;

  if (info.Length() < 3 || !info[0]->IsString() || !info[1]->IsString() || !info[2]->IsString())
  {
    Nan::ThrowTypeError("Bad usage, expected arguments are: path to exe[string], shortcut name[string], app id [string]");
    return;
  }

  HRESULT hr = CoInitializeEx(NULL, COINITBASE_MULTITHREADED);

  if (SUCCEEDED(hr))
  {
    hr = TryCreateShortcut(info[0], info[1], info[2]);
    CoUninitialize();
  }

  if (!SUCCEEDED(hr))
  {
    Nan::ThrowError(Nan::New(hr)->ToString());
    return;
  }
}

void Init(Local<Object> exports) {
  exports->Set(Nan::New("createShortcut").ToLocalChecked(),
               Nan::New<FunctionTemplate>(CreateShortcut)->GetFunction());
}

NODE_MODULE(node_win_shortcut_bindings, Init)
