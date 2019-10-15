#LEDscape Configuration


##JSON Configuration

Configuration info is typically stored in `/etc/ledscape-config.json`. The contents of this file are preserved across software upgrades.  

There are example config files for both WS281x and APA102 LEDs in this directory. 

The `WS281x-config.json` file is used by default on initial installation. 

###Creating a JSON config file from command line arguments

Use the command below to create and execute the JSON configuration

	./opc-server --config ws281x-config.json --mapping rgb-123-v2 --mode ws281x --count 64 --strip-count 48

With this JSON configured it can be called again by issuing the command

	./opc-server --config ws281x-config.json

