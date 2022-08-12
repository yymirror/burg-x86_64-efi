#### List of supported Variables

**GRUB_DEFAULT**
Value: menu item index (0 based) or saved
Default: 0
```
This variable configures the default menu item. When its value is saved, it uses the one stored in burgenv as default. Please note that to achieve the save default effect, you need to set GRUB_DEFAULT to saved, and GRUB_SAVEDEFAULT to true.
```

**GRUB_HIDDEN_TIMEOUT**
Value: any string to indicate true, empty string to indicate false.
Default: empty string (false)
```
If this variable is true, it would not display timeout in menu, but sleep silently. You can use ESC key to interrupt the sleep and display menu normally.
```

**GRUB_HIDDEN_TIMEOUT_QUIET**
Value: true or false.
Default: false
```
This variable works in conjunction with GRUB_HIDDEN_TIMEOUT. When it's true, the hidden timeout is completely silently, otherwise it shows the countdown.
```

**GRUB_TIMEOUT**
Value: timeout in seconds
```
This variable set the timeout value. If it's not set, timeout is infinity.
```

**GRUB_DISTRIBUTOR**
Value: the name of current Linux distribution
```
You can use lsb_release command to extract distribution information, for example:
```
- `GRUB_DISTRIBUTOR=$(lsb_release -i -s 2> /dev/null || echo Debian)`

**GRUB_CMDLINE_LINUX**
**GRUB_CMDLINE_LINUX_DEFAULT**
Value: command line parameter for Linux kernel
```
The difference between GRUB_CMDLINE_LINUX and GRUB_CMDLINE_LINUX_DEFAULT is that GRUB_CMDLINE_LINUX is used in both normal and rescue boot items, while GRUB_CMDLINE_LINUX_DEFAULT is only used in normal boot items.
```

**GRUB_TERMINAL_INPUT**
**GRUB_TERMINAL_OUTPUT**
**GRUB_TERMINAL**
Value: serial, console or gfxterm
Default: gfxterm
```
The input and output terminal driver to use. If input and output driver is the same, you can use GRUB_TERMINAL to set them both. If they're not set, it would try to use gfxterm if video mode is available.
```

**GRUB_FONT**
Value: path to font file
```
If this is not set, it will scan /boot/burg and /usr/share/burg directory for file unicode.pf2, unifont.pf2 or ascii.pf2 and use the first one found as default font file. If none is found, it will use console terminal instead of gfxterm as default.
```

**GRUB_SERIAL_COMMAND**
```
This variable specify the command to setup serial console.
```

**GRUB_DISABLE_LINUX_UUID**
Value: true or false
Default: false
```
If this variable is true, it would use device name in Linux kernel parameter, such as root=/dev/sda1, instead of root=UUID=...
```

**GRUB_DISABLE_LINUX_RECOVERY**
Value: true or false
Default: false
```
If this variable is true, it won't generate Linux boot items for rescue mode.
```

**GRUB_GFXMODE**
Value: graphic mode screen resolution
Default: 640x480
```
You can use vbeinfo command in console windows to find out what modes are supported by video bios.
```

**GRUB_THEME**
Value: theme name or saved (BURG), or path of grub2 theme file (GRUB2)
```
If you use BURG themes, set this to the name of theme, or saved, which means to use the last selected theme. At run-time, you can use 't' hot-key to open a theme selection box and change theme dynamically. If you use GRUB2 themes, set this as the path of theme file.
```

**GRUB_GFXPAYLOAD_LINUX**
Value: keep, text or video mode such as 640x480
Default: keep
```
This variable configures the video mode before switching to Linux. keep means keep the current video mode, text means switch back to text mode. You can use this variable to avoid unnecessary mode switch.
```

**GRUB_DISABLE_OS_PROBER**
Value: true or false
Default: false
```
If it's true, don't run os-prober to detect additional OS.
```

**GRUB_INIT_TUNE**
Value: parameter for the play command
```
If this is set, it would play an initial tune.
```

**GRUB_SAVEDEFAULT**
Value: true or false
Default: false
```
If it's true, save the current selected menu entry to burgenv so that it can be read back at next boot.
```

**GRUB_FOLD** *(BURG only)*
Value: true, false or saved
Default: false
```
You can have normal and rescue boot item for the same kernel, and much more if you have multiple kernels installed. With this variable, you can fold them into one so that it would be easier to select other OS. You can always use 'f' hot-key to switch between folding and unfolding mode. In folding mode, you can also use 'F7' hot-key to pop-up a menu to see the folded items. If this variable is set to saved, it would use the previous selection as default.
```

**GRUB_USERS** *(BURG only)*
`This variable configures password protection for boot items, see`
[Authentication](https://github.com/yymirror/burg-x86_64-efi/blob/main/AUTHENTICATION.md "User-Authentication")
`for more information.`

**GRUB_LINUX16** *(BURG only)*
Value: true or false
Default: false
```
When this is set to true, it'd use the old real mode Linux loader linux16/initrd16 instead of the new protected mode loader.
```

**GRUB_TITLE_OVERRIDE** *(BURG only)*
```
This variable is used to change the menu title of OS detected by os-prober, for example:
```
- `GRUB_TITLE_OVERRIDE="/dev/sda5=Windows 1:/dev/sda6=Windows 2"`

**GRUB_CLASS_OVERRIDE** *(BURG only)*
```
Similar to GRUB_TITLE_OVERRIDE, but this one change OS class. It can be used to set customized icons for selected partition.
```
- `GRUB_CLASS_OVERRIDE="/dev/sda5=win1:/dev/sda6=win2"`
