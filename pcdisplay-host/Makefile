SERVICE_DIR=/usr/lib/systemd/system/

.PHONY: install
install: pcdisplay.service
	sudo cp pcdisplay.service $(SERVICE_DIR)
	sudo systemctl daemon-reload

.PHONY: uninstall
uninstall:
	sudo rm $(SERVICE_DIR)/pcdisplay.service
	sudo systemctl daemon-reload

pcdisplay.service:
	echo "[Unit]" > $@
	echo "Description=PCDisplay service" >> $@
	echo "StartLimitIntervalSec=0" >> $@
	echo "" >> $@

	echo $(USER)

	echo "[Service]" >> $@
	echo "Type=simple" >> $@
	echo "User=$(USER)" >> $@
	echo "Restart=always" >> $@
	echo "WorkingDirectory=$(PWD)" >> $@
	echo "ExecStart=/bin/bash -lc \"ruby $(PWD)/main.rb\"" >> $@

.PHONY: clean
clean:
	rm -f pcdisplay.service
