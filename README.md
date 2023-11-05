# Async TCP Server with ESP32

This project is a simple example of an asynchronous TCP server using the ESP32 microcontroller. It allows you to set up a local server and handle client connections.

## Initial Setup

Before using this project, you need to perform an initial configuration:

1. Rename the `include/config-example.h` file to `include/config.h`.

2. Open the `include/config.h` file and populate the following constants with your network and server details:

```c
#ifndef CONFIG_H
#define CONFIG_H

#define NETWORK_SSID "Your_WiFi_SSID"
#define NETWORK_PASS "Your_WiFi_Password"

#define DNS_PORT 53
#define DNS_HOSTNAME "Your_DNS_Hostname"

#define TCP_SERVER_PORT 8000

#endif
```

## Consuming

To consume this project, I've prepared a simple `Node.js` example. Follow these steps:

- Create a new directory somewhere, and open the directory in your terminal;

- Start a new Node.js project and set it type to `module`:
```sh
npm init -y
npm pkg set type="module"
```

- Create a file named `index.js` at the `root` of your project and paste the following content:
```js
import { Socket } from 'net';
import REPL from 'repl';

const host = '192.168.4.1';
const port = 8000;

const client = new Socket();
client.connect(port, host, () => console.log('Connected to the server'));
client.on('data', data => console.log(`Data received from the server: ${data}`));
client.on('close', () => console.log('Connection closed'));

const send = msg => client.write(`${msg}\n`);

const repl = new REPL.start('> ');
repl.context.send = send;
```

- Start the project by running
```sh
node index.js
```

- After starting the project, you can use the `REPL` to interact with the server. The `send` function allows you to send messages to the server by typing them in the REPL prompt.
```sh
send('cmd')
```
