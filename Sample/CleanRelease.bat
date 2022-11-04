@ECHO OFF
DEL *.sdf *.ncb *.aps *.user *.tmp *.intermediate.manifest *.VC.db /q/s

FOR /F %%x in ('DIR Release /S/B/A:D') DO DEL "%%x\*.*"/Q/S
FOR /F %%x in ('DIR Debug   /S/B/A:D') DO DEL "%%x\*.*"/Q/S

