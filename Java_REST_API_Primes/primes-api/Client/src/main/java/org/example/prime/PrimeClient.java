package org.example.prime;

import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

@Path("primes")
public class PrimeClient {

    public static String REST_URI;

    @Path("checkPrime")
    @GET
    @Produces(MediaType.TEXT_PLAIN)
    public Response isPrime(
            @QueryParam("number") Integer number){
        return Response
                .status(200)
                .entity(PrimeService.getState(number)).build();
    }

    @Path("addNumber")
    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public static Response addNumber(
            @QueryParam("number") Integer number){
        return Response
                .status(200)
                .entity(PrimeService.addNumber(number)).build();
    }

    @Path("listPrimes")
    @GET
    @Produces(MediaType.TEXT_PLAIN)
    public Response listPrime(){
        return Response
                .status(200)
                .entity(PrimeService.getListPrimes()).build();
    }

    @Path("listNonPrimes")
    @GET
    @Produces(MediaType.TEXT_PLAIN)
    public Response listNonPrimes(){
        return Response
                .status(200)
                .entity(PrimeService.getListNonPrimes()).build();
    }
}
