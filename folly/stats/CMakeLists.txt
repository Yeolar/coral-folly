#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_STATS_SRCS
    Instantiations.cpp
)
add_library(folly_stats OBJECT ${FOLLY_STATS_SRCS})


install(FILES
    BucketedTimeSeries-defs.h
    BucketedTimeSeries.h
    Histogram-defs.h
    Histogram.h
    MultiLevelTimeSeries-defs.h
    MultiLevelTimeSeries.h
    TimeseriesHistogram-defs.h
    TimeseriesHistogram.h
    DESTINATION include/folly/stats
)

