FROM mfisherman/mpich:4.2.3 AS builder

USER root

WORKDIR /app

COPY --chmod=777 . /app/triton

RUN apt-get update && apt-get install -y \
    cmake \
    gdal-bin \
    libgdal-dev \
    python3-gdal

RUN rm -rf /app/triton/build && mkdir /app/triton/build
RUN cd /app/triton/build && cmake .. -DMACHINE=Linux
RUN cd /app/triton/build && ./triton_build.sh
RUN for script in run build clean ctest; do \
      echo '#!/bin/bash' > /project/triton_${script}.sh && \
      echo 'cd /app/triton/build && ./triton_'${script}'.sh "$@"' >> /project/triton_${script}.sh && \
      chmod +x /project/triton_${script}.sh; \
    done

FROM mfisherman/mpich:4.2.3 AS runner

USER root

COPY --from=builder /app/triton /app/triton
COPY --from=builder /project /project

RUN apt-get update && apt-get install -y \
    cmake \
    gdal-bin \
    libgdal-dev \
    python3-gdal

RUN chmod +x /project/triton_run.sh
RUN chmod +x /project/triton_build.sh
RUN chmod +x /project/triton_clean.sh
RUN chmod +x /project/triton_ctest.sh
ENV PATH="/project:${PATH}:."
