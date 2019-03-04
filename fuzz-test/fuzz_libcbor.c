#include "cbor.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

const char * key = "a secret key";
bool key_found = false;

void find_string(void * _ctx, cbor_data buffer, size_t len)
{
    if (key_found) {
        printf("Found the value: %*.*s\n", 0, (int) len, buffer);
        key_found = false;
    } else if (len == strlen(key)) {
        key_found = (memcmp(key, buffer, len) == 0);
    }
}


bool fuzz_one_input(const uint8_t *data, size_t size)
{
    struct cbor_callbacks callbacks = cbor_empty_callbacks;
    struct cbor_decoder_result decode_result;
    size_t bytes_read = 0;
    callbacks.string = find_string;
    bool ret = true;
    while (bytes_read < size) {
        decode_result = cbor_stream_decode(data + bytes_read,
                                           size - bytes_read,
                                           &callbacks, NULL);
        if (decode_result.status != CBOR_DECODER_FINISHED) {
            ret = false;
            break;
        }
        bytes_read += decode_result.read;
    }

    return ret;
}
