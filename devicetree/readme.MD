By default, the device tree shipped with recent Linux images has not had the PRU enabled. 

These included `dtb` files enable the PRU and are copied into the `/boot` directory by the installer script. 

###Generating new `dtb` files

The `patch_dts.sh` script will...

1. Determine the correct `dtb` file
2. Decompile a `dtb` file to `dts` source
2. Look for the line that disables the PRU and change it to enable the PRU (`disabled` to `ok`)
3. Recompile the `dts` into a `dtb`. 
4. Back up the existiung `dtb`.
5. Copy the new `dtb` into `/boot`.
