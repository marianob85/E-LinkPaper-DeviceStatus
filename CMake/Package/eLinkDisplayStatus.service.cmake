[Unit] 
Description=Status Project E-Link Display
After=multi-user.target
 
[Service]
Type=simple
ExecStart=/usr/bin/eLinkDisplayStatus /usr/etc/E-LinkStatusConfig.xml
Restart=on-abort
 
[Install]
WantedBy=multi-user.target


