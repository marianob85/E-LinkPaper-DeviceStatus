[Unit]
Description=Status Project E-Link Display
After=multi-user.target
 
[Service]
Type=simple
ExecStart=${CMAKE_INSTALL_PREFIX}/bin/eLinkDisplayStatus ${CMAKE_INSTALL_PREFIX}/etc/E-LinkStatusConfig.xml
Restart=on-abort
 
[Install]
WantedBy=multi-user.target


