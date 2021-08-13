# particle-power-outage
Simple Particle app that supports Particle hardware with a battery input to detect mains power outages.  The software monitors the PWR pin, pushlishing to a cloud variable after detecting a change in state.  The length of the mains outage is published when power is restored.

# Required Hardware
Tested on a [Particle Argon](https://docs.particle.io/datasheets/wi-fi/argon-datasheet/) IoT development board, but supports all Gen 3 Particle hardware.

External LiPo battery, see [batteries](https://docs.particle.io/tutorials/learn-more/batteries/)
