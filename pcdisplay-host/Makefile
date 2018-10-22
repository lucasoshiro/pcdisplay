.PHONY: install
install: pcdisplay.service
	sudo cp pcdisplay.service /etc/systemd/system

.PHONY: uninstall
uninstall:
	sudo rm /etc/systemd/system/pcdisplay.service

pcdisplay.service:
	echo "[Unit]" > $@
	echo "Description=PCDisplay service" >> $@
	echo "After=network.target" >> $@ 	
	echo "StartLimitIntervalSec=0" >> $@
	echo "" >> $@

	echo "[Service]" >> $@
	echo "Type=simple" >> $@
	echo "User=$(USER)" >> $@
	echo "Group=$(USER)" >> $@
#	echo "WorkingDirectory=$(PWD)" >> $@
	echo "Restart=on-failure" >> $@
	echo "ExecStart=/bin/bash -lc \"ruby $(PWD)/main.rb\"" >> $@

	echo "" >> $@
	echo "[Install]" >> $@
	echo "WantedBy=multi-user.target" >> $@


.PHONY: clean
clean:
	rm -f pcdisplay.service
