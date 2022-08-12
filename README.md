## BURG2

*This is GRUB 2, the second version of the GRand Unified Bootloader.*
*GRUB 2 is rewritten from scratch to make GNU GRUB cleaner, safer, more*
*robust, more powerful, and more portable.*

### Preparation
To compile the pc version of BURG, you need install build dependence:

debian/ubuntu:
```
sudo apt-get install autoconf automake bison flex make gcc ruby python gettext libfreetype6-dev
```
fedora:
```
sudo yum install autoconf automake bison flex make gcc ruby python gettext-devel freetype-devel
```

The emulated version requires extra software:

debian/ubuntu:
```
sudo apt-get install libncurses5-dev libsdl1.2-dev*
```
fedora:
```
sudo yum install ncurses-devel SDL-devel
```

In debian/ubuntu, you can also use the following command to install all dependent software:
```
sudo apt-get build-dep burg-pc burg-emu
```

### Compile and install
I recommend using a separate directory for compilation, this makes the
original source tree a lot cleaner. It's also better to use a different
install directory so that it won't overwrite the one installed by apt-get.
In the following example, I uses $HOME/burg_pc and $HOME/burg_emu for compilation,
and $HOME/burg_install as the target install directory.
The source code of BURG is assumed to be in $HOME/burg.

**To compile and install pc version of BURG:**
```
mkdir $HOME/burg_pc
cd $HOME/burg_pc
$HOME/burg/configure --with-platform=pc --prefix=$HOME/burg_install
make
make install
```

**To compile and install emulated version of BURG:**
```
mkdir $HOME/burg_emu
cd $HOME/burg_emu
$HOME/burg/configure --with-platform=emu --prefix=$HOME/burg_install
make
make install
```

### Configure
You need to create a configuration file at $HOME/burg_install/etc/default/burg,
you can copy it from /etc/default/burg, or create from scratch, here is an example:

```
# If you change this file, run
# $> burg-mkconfig -o /boot/burg/burg.cfg
# afterwards to update /boot/burg/burg.cfg.
GRUB_DEFAULT=0
GRUB_TIMEOUT=5
GRUB_DISTRIBUTOR=$(lsb_release -i -s 2> /dev/null || echo Debian)
GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"
GRUB_CMDLINE_LINUX=""

# Uncomment to disable graphical terminal (grub-pc only)
GRUB_TERMINAL=console

# If you want to enable the save default function, uncomment the following
# line, and set GRUB_DEFAULT to saved.
GRUB_SAVEDEFAULT=true

# The resolution used on graphical terminal
# note that you can use only modes which your graphic card supports via VBE
# you can see them in real GRUB with the command `vbeinfo'
# In the boot menu, use hotkey 'r' to popup a resolution selection menu.
GRUB_GFXMODE=saved

# Uncomment if you don't want GRUB to pass "root=UUID=xxx" parameter to Linux
GRUB_DISABLE_LINUX_UUID=true

# Uncomment to disable generation of recovery mode menu entries
GRUB_DISABLE_LINUX_RECOVERY="true"

# Uncomment to get a beep at grub start
GRUB_INIT_TUNE="480 440 1"

# GRUB_THEME's value can be 'saved' or a specific BURG theme name, you can also
# set it to the pathname of a GRUB2 theme file.
# In the boot menu, use hotkey 't' to popup a theme selection menu
GRUB_THEME=saved

# GRUB_FOLD's value can be 'saved', 'true' or 'false'.
# In the boot menu, use hotkey 'F7' to show the full list, 'f' to toggle
# between folding modes.
GRUB_FOLD=saved

# Add user with burg-adduser, then use GRUB_USERS to config authentication.
# The following example means user1 can boot Ubuntu, no password is needed to
# boot Windows, user1 amd user2 can boot other OS. Superusers can boot any OS
# and use hotkeys like `c' to enter console mode.
GRUB_USERS="*=user1,user2:ubuntu=user1:windows="

#EOF
```
*For a complete list of supported variables, refer to this document:*
- [Supported Variables](https://github.com/yymirror/burg-x86_64-efi/blob/main/VARIABLES.md "List of supported variables")


If you have customized boot items, you need to copy
*/etc/burg.d/40_custom* to *$HOME/burg_install/etc/burg.d/40_custom*.

Now, install it to MBR so that it will take effect on the next boot:
```
sudo $HOME/burg_install/sbin/burg-install /dev/sda
sudo $HOME/burg_install/sbin/burg-mkconfig -o /boot/burg/burg.cfg
```

To switch back to default BURG, just run:
```
sudo burg-install /dev/sda
sudo update-burg -o /boot/burg/burg.cfg
```
