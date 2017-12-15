#include "EncoderConfig.h"
#include "Logger.h"

EncoderConfig::EncoderConfig(const std::string &filePath) : Config(filePath),
                                                            rawfile(getString("rawfile")),
                                                            encfile(getString("encfile")),
                                                            width(getInt("width")),
                                                            height(getInt("height")),
                                                            rle(getBool("rle")),
                                                            quantfile(getString("quantfile")),
                                                            logfile(getString("logfile")),
                                                            gop(getInt("gop")),
                                                            merange(getInt("merange")) {}
