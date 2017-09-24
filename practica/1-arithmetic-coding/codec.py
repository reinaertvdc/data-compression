import decimal
from decimal import Decimal
import json
import re
import sys


decimal.getcontext().prec = 200


def binStringToDecimalFraction(binStr):
    # TODO calculate fraction
    return Decimal('0.0')


def decodeDecimalFraction(value, alphabetInfo):
    # TODO decode
    return ''


def encodeToRange(text):
    if len(text) > 100:
        raise ValueError('Input string is longer than 100 characters')
    elif not re.match(r'^[a-z ]*$', text):
        raise ValueError('Input string contains characters other than lowercase a-z and space')

    # TODO determine range and alphabet info
    bottom = Decimal('0.0')
    top = Decimal('0.0')
    alphabetInfo = []

    return (bottom, top, alphabetInfo)


def generateBinaryStringInRange(bottom, top):
    # TODO generate binary string
    return ''


def getAlphabetFractions(alphabetInfo):
    rangeInfo = {}
    totalLength = 0

    for character, frequency in alphabetInfo:
        # TODO calculate range
        bottom = Decimal('0.0')
        top = Decimal('0.0')

        rangeInfo[character] = {
            'bottom': bottom,
            'top': top,
        }

        totalLength += frequency

    return (rangeInfo, totalLength)


if __name__ == '__main__':
    mode = sys.argv[1]
    inputFilePath = sys.argv[2]

    with open(inputFilePath) as inputFile:
        inputString = inputFile.read().replace('\r\n', '').replace('\n', '')

    if mode == 'test':
        bottom, top, alphabetInfo = encodeToRange(inputString)
        mean = (bottom + top) / 2
        outputString = decodeDecimalFraction(mean, alphabetInfo)

        print(' INPUT: "' + inputString + '"')
        print('OUTPUT: "' + outputString + '"')
        print('They are' + ('' if inputString == outputString else ' NOT') + ' identical.')
    elif mode == 'encode':
        outputFilePath = sys.argv[3]
        alphabetInfoFilePath = sys.argv[4]

        bottom, top, alphabetInfo = encodeToRange(inputString)
        binaryString = generateBinaryStringInRange(bottom, top)

        with open(outputFilePath) as outputFile:
            outputFile.write(binaryString)
        
        with open(alphabetInfoFilePath) as alphabetInfoFile:
            json.dump(alphabetInfo, alphabetInfoFile)
    elif mode == 'decode':
        alphabetInfoFilePath = sys.argv[3]
        outputFilePath = sys.argv[4]

        with open(alphabetInfoFilePath) as alphabetInfoFile:
            alphabetInfo = json.load(alphabetInfoFile)
        
        fraction = binStringToDecimalFraction(inputString)
        outputString = decodeDecimalFraction(fraction, alphabetInfo)

        with open(outputFilePath) as outputFile:
            outputFile.write(outputString)
