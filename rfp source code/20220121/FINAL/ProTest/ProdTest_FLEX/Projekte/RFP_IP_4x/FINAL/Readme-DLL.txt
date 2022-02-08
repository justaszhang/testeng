Achtung:

alle *.DLL Dateien müssen im Stamverzeichniss des Programmes liegen.

Windows search Pfad für die DLL ist

1.) Windows System:   "C:\Windows\System32"
2.) im aktuellen Verzeichniss beim start des Programmes (kann mit PWD oder curentdir angezeigt werden)
2.) im Verzeichniss des Hauptprogrammes
3.) die Verzeichnisse der Environment Search Variable PATH


CVI 6.0 kann die PATH Variable nicht ändern, also keine Option...

zur WinSocket DLL gehoeren:
WS2_32.lib
ws2_32.dll
WINSOCK.H


zur libssh DLL gehoeren:
ssh.dll
ssh.lib
libssh.h
callbacks.h
legacy.h
server.h
sftp.h
ssh2.h

libeay32.dll
msvcr100d.dll
zlib1.dll
