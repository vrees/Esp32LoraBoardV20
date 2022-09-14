  # Update from TTN V2 to TTN V3


### Firmware
https://downloads.rakwireless.com/LoRa/RAK2245-Pi-HAT/Firmware/RAK2245_Latest_Firmware.zip
https://docs.rakwireless.com/Product-Categories/WisLink/RAK2245-Pi-HAT/Quickstart/#prerequisites
https://downloads.rakwireless.com/en/LoRa/RAK2245-Pi-HAT/Hardware-Specification/RAK2245_RAK831_Design_Guide_Comparison.pdf

- Image installieren  mit rpi-imager

### Was necessary to reset SX1301 chip by using Pin GPIO 25 because my board from Charles Hallard uses Pin 25 for reset of SX1301

```
sudo vi /opt/ttn-gateway/packet_forwarder/lora_pkt_fwd/start.sh

change 
   SX1301_RESET_BCM_PIN=17
to
   SX1301_RESET_BCM_PIN=25   
```

Schema of the board from Charles Hallard: https://raw.githubusercontent.com/hallard/RAK831-Zero/master/pictures/PiZero-RAK-sch.png
https://github.com/hallard/RAK831-Zero/

```
cd /opt/ttn-gateway/lora_gateway
./reset_lgw.sh start 25
./reset_lgw.sh stop 25

sudo journalctl -f
```


##  local_conf.json

Entweder Datei löschen oder verifizieren ob korrekte GatewayId eingetragen. 

Falls gelöscht: sicherstellen dass die GatewayId in global_conf.json korrekt ist


__sudo vi /opt/ttn-gateway/packet_forwarder/lora_pkt_fwd/local_conf.json__

{
        "gateway_conf": {
                "gateway_ID": "B827EBFFFE6DF594"
        }
}


## Check Gateway EUI
```
$ gateway-version 
Raspberry Pi Zero W Rev 1.1, OS "10 (buster)", 4.19.97+.
RAKWireless gateway RAK7243 no LTE version 4.2.5R install from firmware.
Gateway ID: B827EBFFFE6DF594.
```

## /opt/ttn-gateway/packet_forwarder/lora_pkt_fwd/global_conf.json

```
{
    "SX1301_conf": {
        "lorawan_public": true,
        "clksrc": 1,
        "antenna_gain": 0,
        "radio_0": {
            "enable": true,
            "type": "SX1257",
            "freq": 867500000,
            "rssi_offset": -166.0,
            "tx_enable": true,
            "tx_freq_min": 863000000,
            "tx_freq_max": 870000000
        },
        "radio_1": {
            "enable": true,
            "type": "SX1257",
            "freq": 868500000,
            "rssi_offset": -166.0,
            "tx_enable": false
        },
        "chan_multiSF_0": {
            "enable": true,
            "radio": 1,
            "if": -400000
        },
        "chan_multiSF_1": {
            "enable": true,
            "radio": 1,
            "if": -200000
        },
        "chan_multiSF_2": {
            "enable": true,
            "radio": 1,
            "if": 0
        },
        "chan_multiSF_3": {
            "enable": true,
            "radio": 0,
            "if": -400000
        },
        "chan_multiSF_4": {
            "enable": true,
            "radio": 0,
            "if": -200000
        },
        "chan_multiSF_5": {
            "enable": true,
            "radio": 0,
            "if": 0
        },
        "chan_multiSF_6": {
            "enable": true,
            "radio": 0,
            "if": 200000
        },
        "chan_multiSF_7": {
            "enable": true,
            "radio": 0,
            "if": 400000
        },
        "chan_Lora_std": {
            "enable": true,
            "radio": 1,
            "if": -200000,
            "bandwidth": 250000,
            "spread_factor": 7
        },
        "chan_FSK": {
            "enable": true,
            "radio": 1,
            "if": 300000,
            "bandwidth": 125000,
            "datarate": 50000
        },
        "tx_lut_0": {
            "pa_gain": 0,
            "mix_gain": 9,
            "rf_power": -6,
            "dig_gain": 0
        },
        "tx_lut_1": {
            "pa_gain": 0,
            "mix_gain": 12,
            "rf_power": -3,
            "dig_gain": 0
        },
        "tx_lut_2": {
            "pa_gain": 0,
            "mix_gain": 15,
            "rf_power": 0,
            "dig_gain": 0
        },
        "tx_lut_3": {
            "pa_gain": 1,
            "mix_gain": 8,
            "rf_power": 3,
            "dig_gain": 1
        },
        "tx_lut_4": {
            "pa_gain": 1,
            "mix_gain": 9,
            "rf_power": 6,
            "dig_gain": 0
        },
        "tx_lut_5": {
            "pa_gain": 1,
            "mix_gain": 11,
            "rf_power": 10,
            "dig_gain": 0
        },
        "tx_lut_6": {
            "pa_gain": 1,
            "mix_gain": 12,
            "rf_power": 11,
            "dig_gain": 1
        },
        "tx_lut_7": {
            "pa_gain": 1,
            "mix_gain": 12,
            "rf_power": 12,
            "dig_gain": 0
        },
        "tx_lut_8": {
            "pa_gain": 2,
            "mix_gain": 11,
            "rf_power": 13,
            "dig_gain": 1
        },
        "tx_lut_9": {
            "pa_gain": 2,
            "mix_gain": 11,
            "rf_power": 14,
            "dig_gain": 0
        },
        "tx_lut_10": {
            "pa_gain": 3,
            "mix_gain": 8,
            "rf_power": 16,
            "dig_gain": 1
        },
        "tx_lut_11": {
            "pa_gain": 3,
            "mix_gain": 8,
            "rf_power": 20,
            "dig_gain": 0
	},
	"tx_lut_12": {
		"desc": "TX gain table, index 12",
		"pa_gain": 3,
		"mix_gain": 12,
		"rf_power": 23,
		"dig_gain": 1
	},
	"tx_lut_13": {
		"desc": "TX gain table, index 13",
		"pa_gain": 3,
		"mix_gain": 13,
		"rf_power": 25,
		"dig_gain": 0
	},
	"tx_lut_14": {
		"desc": "TX gain table, index 14",
		"pa_gain": 3,
		"mix_gain": 13,
		"rf_power": 26,
		"dig_gain": 0
	},
	"tx_lut_15": {
		"desc": "TX gain table, index 15",
		"pa_gain": 3,
		"mix_gain": 13,
		"rf_power": 27,
		"dig_gain": 0
        }
    },
	"gateway_conf": {
    		"gateway_ID": "B827EBFFFE6DF594",
		/* change with default server address/ports, or overwrite in local_conf.json */
		"server_address": "eu1.cloud.thethings.network",
		"serv_port_up": 1700,
		"serv_port_down": 1700,
		/* adjust the following parameters for your network */
		"keepalive_interval": 10,
		"stat_interval": 30,
		"push_timeout_ms": 100,
		/* forward only valid packets */
		"forward_crc_valid": true,
		"forward_crc_error": false,
		"forward_crc_disabled": false,
		/* gps enable */
		/* "gps_tty_path": "/dev/ttyAMA0", */
		"fake_gps": true,
		"ref_latitude": 47.897088,
		"ref_longitude": 7.896856,
		"ref_altitude": 1100, 
		"autoquit_threshold": 20
	}
}
```
