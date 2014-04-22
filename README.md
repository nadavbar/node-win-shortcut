node-win-shortcut
=================

Node.js native module for creating shortcuts (including app-ids) in windows > 8.
The native module code which creates the shortcuts is based on the samples from: http://msdn.microsoft.com/en-us/library/windows/desktop/hh802762

Usage:
First, clone this repo and **run node-gyp rebuild** from inside the cloned directory.
In order to create a shortcut, require this module, and call the **createShortcut** function which accepts the following parameters: <br>
  1. Path to the executable for which the shortcut will be created.<br>
  2. The name of the shortcut.<br>
  3. The app id that the shortcut will be created with. (a string of your choice, just make sure it's unique from other app ids)<br>

Here's an example:
```javascript
// create a shortcut for node
var win_shortcut = require('node-win-shortcut');
win_shortcut.createShortcut(process.execPath, 'node', 'node_app_id');
```
