[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=FEXCYUUW2GCLG&lc=US&item_name=namreeb%2eorg&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHosted)

nampower
========

An auto stop-cast tool for World of Warcraft 1.12.1.5875 (for Windows)

There is a design flaw in this version of the client.  A player is not allowed to cast a
second spell until after the client receives word of the completion of the previous spell.
This means that in addition to the cast time, you have to wait for the time it takes a
message to arrive from the server.  For many U.S. based players connected to E.U. based
realms, this can result in approximately a 20% drop in effective DPS.

Consider the following timeline, assuming a latency of 200ms.

* t = 0, the player begins casting fireball (assume a cast time of one second or 1000ms)
       and spell cast message is sent to the server.  at this time, the client places
       a lock on itself, preventing the player from requesting another spell cast.
* t = 200, the spell cast message arrives at the server, and the spell cast begins
* t = 1200, the spell cast finishes and a finish message is sent to the client
* t = 1400, the client receives the finish message and removes the lock it had placed
          1400ms ago.
		  
In this scenario, a 1000ms spell takes 1400ms to cast.  This tool will work around that
design flaw by bypassing the lock the client places on itself during a spell cast.

To cast a spell at your current target using this tool, create a macro of the format
"/script CastSpellAtTarget(x)" where x is the numerical id of the spell you wish to cast.

To cast a spell at your current mouseover target using this tool, create a macro of the
format "/script CastSpellAtMouseover(x)" where x is the numerical id of the spell you
wish to cast.

To launch, run loader.exe -p c:\path\to\wow.exe (or just loader.exe with it inside the
main wow folder)

Note that while this makes no malicious changes to the WoW client, it could easily be
mistaken as malicious by the primitive anticheats in use on most vanilla private
servers.  This program contains absolutely no protection against anticheat software.

Feenix / wow-one.com has said that they will not ban people for using it.  Refer to
this thread: http://www.wow-one.com/forum/topic/88974-will-running-this-mod-get-people-banned/

Kronos / Twinstar has said that while they do not support client modification, they
will not specifically target this mod.  Refer to this thread:
http://forum.twinstar.cz/showthread.php/97154-Planning-to-release-a-wow-mod-Will-it-get-people-banned

Crestfall has said they have no issue with people using it unless it later is revealed to
enable abuse.  Refer to this thread: http://forums.crestfall-gaming.com/index.php?/topic/1224-will-nampower-be-allowed/

USE AT YOUR OWN RISK