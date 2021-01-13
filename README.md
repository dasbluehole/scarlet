# scarlet
A small and tiny HTTP server for testing
Based on https://github.com/OminiaVincit/http_server (now Heavily Modified)
#### Currently it supports Static files(html,CSS,js,plain text) and user defined function through routing.
User functions are defined in <b>myapp.h</b>. They are registered in <b>main function in scarlet.c</b> by using <b>add_route()</b> function.

This one is very experimental though. Read the code and use it for fun.
it is a very small <b><i>Web Application framework</i></b> like Bottle or flask or may be something else.
Who knows.... better you  suggest and try ;)
Bellow is the Output of httperf on this server.

### 3000 connections per second!!!

[ashok@localhost ~]$ httperf --server localhost --port 8080 --uri /    --rate 3000 --num-conn 16000 --num-call 1 --timeout 10
httperf --timeout=10 --client=0/1 --server=localhost --port=8080 --uri=/ --rate=3000 --send-buffer=4096 --recv-buffer=16384 --num-conns=16000 --num-calls=1
httperf: warning: open file limit > FD_SETSIZE; limiting max. # of open files to FD_SETSIZE
Maximum connect burst length: 6

Total: connections 16000 requests 16000 replies 16000 test-duration 5.794 s

Connection rate: 2761.7 conn/s (0.4 ms/conn, <=59 concurrent connections)
Connection time [ms]: min 0.3 avg 6.8 max 3105.0 median 0.5 stddev 84.9
Connection time [ms]: connect 5.2
Connection length [replies/conn]: 1.000

Request rate: 2761.7 req/s (0.4 ms/req)
Request size [B]: 62.0

Reply rate [replies/s]: min 2999.1 avg 2999.1 max 2999.1 stddev 0.0 (1 samples)
Reply time [ms]: response 1.5 transfer 0.1
Reply size [B]: header 74.0 content 2762.0 footer 0.0 (total 2836.0)
Reply status: 1xx=0 2xx=16000 3xx=0 4xx=0 5xx=0

CPU time [s]: user 0.79 system 3.49 (user 13.6% system 60.2% total 73.8%)
Net I/O: 7815.9 KB/s (64.0*10^6 bps)

Errors: total 0 client-timo 0 socket-timo 0 connrefused 0 connreset 0
Errors: fd-unavail 0 addrunavail 0 ftab-full 0 other 0
