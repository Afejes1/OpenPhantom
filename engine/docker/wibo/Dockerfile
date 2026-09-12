# Build context contains only the separately downloaded, verified wibo-i686.
# No Microsoft compiler or game files are copied into this image.
FROM debian:bookworm-slim@sha256:88200866dfff7ea7f5cbcb6ec7c8a701889efe6fe859fe64d6990e4b07ea4171
COPY --chmod=0755 wibo-i686 /usr/local/bin/wibo
RUN echo "2575d3b0a2f408b2c2b0850db56f1af5d005a138394a6774eba77b6708ecc304  /usr/local/bin/wibo" | sha256sum -c -
USER 65534:65534
ENTRYPOINT ["/usr/local/bin/wibo"]
