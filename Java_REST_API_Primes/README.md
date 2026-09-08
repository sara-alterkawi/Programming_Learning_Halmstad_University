# T3-REST-API-Primes
Using the REST API, implement a small prime numbers web service.
The server holds a database of prime and non-prime numbers (the "database" can be implemented any way you choose, simple text file or even a RAM stored list is sufficient).
Clients can send requests with a number to check if it is registered in either of the two classes. If not, the client calculates the primeness (use any algorithm or existing API you wish, including the naive version shown during the lecture on Java concurrency) for the number and sends a request to the server to store it in the server’s records.
You can use the Jersey library for Java to do this, or any other REST API technology, including other programming languages (if you choose to do so, you have to do your research on this, for Python there is a link in the Practicalities).
One important thing to keep in mind that you should use the correct REST API operations, that is GET for retrieving the numbers from the server and PUT or POST of submitting them.
