-----BEGIN MPKI CREDENTIAL-----

-----BEGIN PUBLIC KEY-----
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCvC+m32GusrDvN5tQyAlr7SEcg
cyNTfLSHG14bgX2CQzz6z5saoYGZWI3blurN4M+yTLlYEQGcI5bqqoFy40V7b2UF
+UtU7hlXF+0041qDgN6iQGca19mizpBsdHycYgR4NaXcEt1P3JNOczX9HDTqdXSP
1wECQ1TmlDRLCFoqSwIDAQAB
-----END PUBLIC KEY-----

-----BEGIN MPKI CERTIFICATE-----

allow(PK, Access, Resources) :-
	    pk_bind(P, PK),
	    allow(P, Access, Resources).

pk_bind(jeremiah, rsa_3fcb4a57240d9287e43b8615e9994bba).

allow(jeremiah, read, file1).

allow(jeremiah, _anyAccess, file2).

allow(jeremiah, read, file3).

-----BEGIN VALIDITY-----
Not before: datime(2010,10,12,0,0,0).
Not after: datime(2011,12,19,0,0,0).
-----END VALIDITY-----

-----BEGIN PUBLIC KEY-----
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQD6gkiTvzHRJQviUXnlK4aO2U7j
M14MU3fyKvom7yTRrjq5cwVgtIEZb6ZFTX6wM4y4ii3r1qjoyuIKQwYV0heHswe6
jmoAmGJyTNY1dtrshcXN6N3Co5zXCShmQdt/y3bUDl9/rrVHatAAwDhC9/RwecjG
pV24BPuAEjIJfa1gVwIDAQAB
-----END PUBLIC KEY-----

-----BEGIN SIGNATURE-----
UYVSB8O8RJKH4QB4y5TzQuL3yPRj3gicDeDYs3SJp2wXsHPklxMire/S2LSkZBEN
AO8FguTGul0QNFZNbGprypBjOVab84FbVMI0ZM15iLxYSJz9ozyPfgfIIk0TS20y
cQKX7XRPqQtZBmGxG4vou1eBAOlimzccPgNfPSNoQUQ=
-----END SIGNATURE-----

-----END MPKI CERTIFICATE-----

-----END MPKI CREDENTIAL-----
