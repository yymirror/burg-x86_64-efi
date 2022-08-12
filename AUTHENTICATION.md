**First**, *configure user and passwords*

To add/edit user, use burg-adduser, for example:

`sudo burg-adduser --super admin`
`sudo burg-adduser user1`

*The --super option is used to specify super user.*


To remove user, use burg-deluser, for example:

`sudo burg-deluser user1`

*The password is stored in /etc/default/burg-passwd.*


**Second**, *setup GRUB_USERS variable in /etc/default/burg*

Here is an example:

```
# /etc/default/burg
GRUB_USERS="*=user1,user2:ubuntu=user1:windows="
```

This means user1 can boot Ubuntu, no password is needed for Windows,
user1 and user2 can boot other OS besides Ubuntu and Windows.
Superusers can boot any OS and use system-wide hotkeys like `c'
to enter console mode.(**!**)

You can also use different setting for normal and rescue boot items, add to class name to indicate rescue items, for example:

- `GRUB_USERS="ubuntu=user1"` user1 can use both normal and rescue boot items.

- `GRUB_USERS="*ubuntu=user1"` Anyone can use normal boot item, user1 can use rescue boot item.

- `GRUB_USERS="ubuntu=user1:*ubuntu=user2"` User1 can use normal boot item, user2 can use rescue boot item.

Superusers can use all items.(**!**)

Finally, generate burg.cfg with update-burg:

`sudo update-burg`

**You can use burg-emu to check the configuration before reboot:**

`sudo burg-emu`
