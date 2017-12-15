#include "DecoderConfig.h"

DecoderConfig::DecoderConfig(const std::string &filePath) : Config(filePath),
                                                            decfile(getString("decfile")),
                                                            encfile(getString("encfile")),
                                                            logfile(getString("logfile")),
                                                            motioncompensation(getBool("motioncompensation")) {}
