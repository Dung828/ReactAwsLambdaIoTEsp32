#ifndef certs_h
#define certs_h

// Amazon's root CA. This should be the same for everyone.
const char AWS_CERT_CA[] = R"(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)";

// The private key for your device
const char AWS_CERT_PRIVATE[] = R"(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)";

// The certificate for your device
const char AWS_CERT_CRT[] = R"(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)";
#endif
