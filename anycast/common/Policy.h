/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-18T21:43:50-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Anycast.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-01T17:02:44-04:00
 */
enum Policy{
    Proximity = 1,
    LoadBanlancing = 2
};

enum ServerType {
    JAP, // Join Anycast Proxy
    RAP, // Rendezvous Anycast Proxy
    IAP, // Ingress Anycast Proxy
    AT // Anycast Target
};
