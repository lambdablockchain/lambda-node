# Shared Libraries

## lambdaconsensus

The purpose of this library is to make the verification functionality that is
critical to Lambda's consensus available to other applications, e.g. to
language bindings.

### API

The interface is defined in the C header `lambdaconsensus.h` located in
`src/script/lambdaconsensus.h`.

#### Version

`lambdaconsensus_version` returns an `unsigned int` with the API version
*(currently at an experimental `0`)*.

#### Script Validation

`lambdaconsensus_verify_script` returns an `int` with the status of the verification.
It will be `1` if the input script correctly spends the previous output `scriptPubKey`.

##### Parameters

- `const unsigned char *scriptPubKey` - The previous output script that encumbers
  spending.
- `unsigned int scriptPubKeyLen` - The number of bytes for the `scriptPubKey`.
- `const unsigned char *txTo` - The transaction with the input that is spending
  the previous output.
- `unsigned int txToLen` - The number of bytes for the `txTo`.
- `unsigned int nIn` - The index of the input in `txTo` that spends the `scriptPubKey`.
- `unsigned int flags` - The script validation flags *(see below)*.
- `lambdaconsensus_error* err` - Will have the error/success code for the
  operation *(see below)*.

##### Script Flags

- `lambdaconsensus_SCRIPT_FLAGS_VERIFY_NONE`
- `lambdaconsensus_SCRIPT_FLAGS_VERIFY_P2SH` - Evaluate P2SH ([BIP16](https://github.com/lambda/bips/blob/master/bip-0016.mediawiki))
  subscripts
- `lambdaconsensus_SCRIPT_FLAGS_VERIFY_DERSIG` - Enforce strict DER ([BIP66](https://github.com/lambda/bips/blob/master/bip-0066.mediawiki))
  compliance
- `lambdaconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY` - Enable CHECKLOCKTIMEVERIFY
  ([BIP65](https://github.com/lambda/bips/blob/master/bip-0065.mediawiki))
- `lambdaconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY` - Enable CHECKSEQUENCEVERIFY
  ([BIP112](https://github.com/lambda/bips/blob/master/bip-0112.mediawiki))
- `lambdaconsensus_SCRIPT_FLAGS_VERIFY_WITNESS` - Enable WITNESS ([BIP141](https://github.com/lambda/bips/blob/master/bip-0141.mediawiki))
- `lambdaconsensus_SCRIPT_ENABLE_SIGHASH_FORKID` - Enable SIGHASH_FORKID replay
  protection ([UAHF](https://gitlab.com/lambdablockchain/bchn-sw/lambda-upgrade-specifications/-/blob/master/spec/uahf-technical-spec.md#req-6-2-mandatory-signature-shift-via-hash-type))

##### Errors

- `lambdaconsensus_ERR_OK` - No errors with input parameters *(see the return
  value of `lambdaconsensus_verify_script` for the verification status)*
- `lambdaconsensus_ERR_TX_INDEX` - An invalid index for `txTo`
- `lambdaconsensus_ERR_TX_SIZE_MISMATCH` - `txToLen` did not match with the size
  of `txTo`
- `lambdaconsensus_ERR_DESERIALIZE` - An error deserializing `txTo`
- `lambdaconsensus_ERR_AMOUNT_REQUIRED` - Input amount is required if WITNESS is
  used

### Example Implementations

- [Lambda Node (BCHN)](https://github.com/lambdablockchain/lambda-node/-/blob/master/src/script/lambdaconsensus.h)
- [Lambda Unlimited (BUCash)](https://github.com/LambdaUnlimited/LambdaUnlimited/blob/release/src/script/lambdaconsensus.h)

### Historic Example Implementations in Lambda (pre-dating Lambda)

- [Lambda Core](https://github.com/lambda/lambda/blob/master/src/script/lambdaconsensus.h)
- [NLambda](https://github.com/NicolasDorier/NLambda/blob/master/NLambda/Script.cs#L814)
  (.NET Bindings)
- [node-liblambdaconsensus](https://github.com/bitpay/node-liblambdaconsensus)
  (Node.js Bindings)
- [java-liblambdaconsensus](https://github.com/dexX7/java-liblambdaconsensus)
  (Java Bindings)
- [lambdaconsensus-php](https://github.com/Bit-Wasp/lambdaconsensus-php) (PHP Bindings)
