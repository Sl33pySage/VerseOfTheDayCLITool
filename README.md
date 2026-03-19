# VerseOfTheDayCLITool

A fun low level project that grabs a verse of the day and displays it in the terminal. Thinking of it being either on command or everytime a terminal session opens.

# Things-to-do/consider

Socket Programming (The Network Layer): Use the <sys/socket.h> library to manually open a connection to the API server. You'll need to:
*Perform a DNS lookup to find the server's IP address.
*Craft a raw HTTP GET request string (e.g., GET /john+3:16 HTTP/1.1\r\nHost:bible-api.com\r\n\r\n).
\*Manage the byte-by-byte transfer of data over the connection.
