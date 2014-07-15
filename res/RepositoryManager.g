grammar RepositoryManager;

options
{
    language=C;
}

@parser::header {
}
@lexer::header {
}


credential
	:	CREDENTIAL_HEADER public_key certificate* CREDENTIAL_END
	;
public_key
	:	PUBLIC_KEY_HEADER base64 PUBLIC_KEY_END
	;
signature
	:	SIGNATURE_HEADER base64 SIGNATURE_END
	;
validity
	:	VALIDITY_HEADER NOT_BEFORE time_tuple NOT_AFTER time_tuple VALIDITY_END
	;
time_tuple
	:	'datime' '(' num ',' num ',' num ',' num ',' num ',' num ')'  '.'
	;
certificate
	:	CERT_HEADER clause+ validity public_key signature CERT_END
	;
clause
	:	literal '.'
	|	literal ':-' literal
			( ',' literal)* '.'
	;
literal
	:	predicate
	|	'says' '(' text ',' predicate ')'
	;
predicate
	:	text  '(' text
		( ','text)* ')'
	;
challenge
	:	CHALLENGE_HEADER text CHALLENGE_END 
    ;
response 
	:	RESPONSE_HEADER base64 RESPONSE_END
    ;
request
	:	REQUEST_HEADER 'request' '(' op ',' text ')' REQUEST_END 
	;
op	
    :	text
    ;
reply
    :	REPLY_HEADER result REPLY_END 
    ;
result	
    :	text 
    ;
text 
	:	TEXT
	;
base64
	:	text+
	;
num
	:	NUM
    ;
WS      :	(' '|'\t'|'\n'|'\r')+ {_channel=99;}
	;

NUM	: 	('0' ..'9')*
	;

TEXT	:	('a'..'z'|'A'..'Z'|'0'..'9'|'/'|'+'|'='|'_'|'-'|':')* 
	;
	
CREDENTIAL_HEADER:'-----BEGIN MPKI CREDENTIAL-----' ;
CREDENTIAL_END:'-----END MPKI CREDENTIAL-----' ;
PUBLIC_KEY_HEADER:'-----BEGIN PUBLIC KEY-----' ;
PUBLIC_KEY_END:'-----END PUBLIC KEY-----' ;
SIGNATURE_HEADER:'-----BEGIN SIGNATURE-----' ;
SIGNATURE_END:'-----END SIGNATURE-----' ;
VALIDITY_HEADER:'-----BEGIN VALIDITY-----' ;
VALIDITY_END:'-----END VALIDITY-----' ;
CERT_HEADER:'-----BEGIN MPKI CERTIFICATE-----' ;
CERT_END:'-----END MPKI CERTIFICATE-----' ;
CHALLENGE_HEADER:'-----BEGIN MPKI CHALLENGE-----' ;
CHALLENGE_END:'-----END MPKI CHALLENGE-----' ;
RESPONSE_HEADER:'-----BEGIN MPKI CHALLENGE RESPONSE-----' ;
RESPONSE_END:'-----END MPKI CHALLENGE RESPONSE-----' ;
REQUEST_HEADER:'-----BEGIN REPOSITORY CLIENT REQUEST-----' ;
REQUEST_END:'-----END REPOSITORY CLIENT REQUEST-----' ;
REPLY_HEADER:'-----BEGIN REPOSITORY SERVER REPLY-----' ;
REPLY_END:'-----END REPOSITORY SERVER REPLY-----' ;
NOT_BEFORE
	:	'Not before:';
NOT_AFTER
	:	'Not after:';	
	

