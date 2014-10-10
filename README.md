node-win-shortcut
=================

Node.js native module for creating shortcuts (including app-ids) in windows > 8.
The native module code which creates the shortcuts is based on the samples from: http://msdn.microsoft.com/en-us/library/windows/desktop/hh802762

The app-id which will be created with the shortcut can be used to show desktop notificatons with 
the <a href="https://github.com/NodeRT/NodeRT" target="_blank">NodeRT</a> module: windows.ui.notifications.
Please take a look at <a href=""https://github.com/NodeRT/NodeRT/tree/master/samples/notification target="_blank">this sample</a> for more details.

Usage:
In order to create a shortcut, require this module, and call the **createShortcut** function which accepts the following parameters: 
    1. Path to the executable for which the shortcut will be created.
    2. The name of the shortcut. 
    3. The app id that the shortcut will be created with. (a string of your choice)

Example:
```javascript
// create a shortcut for node
var win_shortcut = require('node-win-shortcut');
win_shortcut.createShortcut(process.execPath, 'node', 'node_app_id');
```

License:
MIT

Author:
nadavbar