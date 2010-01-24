#!/bin/bash

./dns-spoof -p 9797
dig @localhost -p 9797 textureandlight.com
