sudo cp machyXserver.service /etc/systemd/system/
sudo cp machyXserver.sh /usr/local/bin/machyXserver.sh
sudo chmod +x /usr/local/bin/machyXserver.sh
sudo systemctl enable machyXserver.service
sudo systemctl start machyXserver.service