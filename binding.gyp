{
  "targets": [
    {
      "target_name": "node_win_shortcut_bindings",
      "sources": [ "node_win_shortcut_bindings.cc"],
      "include_dirs": [ "<!(node -e \"require('nan')\")" ],
      "libraries": [ "-lruntimeobject.lib","-lshlwapi.lib" ]
    }
  ]
}
