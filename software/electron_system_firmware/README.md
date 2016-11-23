How to Flash Electron Firmware OTA
==================================

* Download parts of desired firmware release from: https://github.com/spark/firmware/releases  There are usually 2-3 parts of ``*.bin`` firmware that need to be downloaded.

* With a functional and logged-in particle CLI client, you can then flash the device over the air with the syntax: ``particle flash $DEVICE_ID $FIREMWARE_PART#.bin``, where ``$DEVICE_ID`` is the electron (by name or numeric ID).  You need to execute this command sequentially for each "part" of the binary firmware.

* The device should automatically reboot after each incremental flash.

