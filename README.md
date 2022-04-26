# Experiment

1. Run `./wserver` in `concurrency-webserver/src`
2. Open 3 browser tabs set to `localhost:5000/spin.cgi?30`
3. Attempt to open a fourth tab set to `localhost:5000/spin.cgi?30`
4. This new tab should time out as the thread buffer has been surpassed
