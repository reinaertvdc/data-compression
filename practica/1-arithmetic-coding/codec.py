import decimal
from decimal import Decimal
import json
import re
import sys


decimal.getcontext().prec = 200


def binStringToDecimalFraction(binStr):
    sum = Decimal('0.0')
    for i, c in enumerate(binStr):
        if c == '1':
            sum += Decimal('2')**Decimal(str(-(i+1)))
    return sum


def decodeDecimalFraction(value, alphabetInfo):
    fractions, totalLength = getAlphabetFractions(alphabetInfo)
    s = ''
    bottom = Decimal('0')
    top = Decimal('1')
    range = top - bottom
    while len(s) < totalLength:
        for key, val in fractions.items():
            range_low = val['bottom']
            range_high = val['top']
            if range_low <= value < range_high:
                s += key
                break
        bottom = range_low
        top = range_high
        range = top - bottom
        value = (value - bottom) / range
    return s


def getAlphabetInfo(text):
    result = dict()
    for c in text:
        if c in result:
            result[c] += 1
        else:
            result[c] = 1
    out = []
    for c in sorted(result):
        out.append([c, result[c]])
    return out


def encodeToRange(text):
    if len(text) > 100:
        raise ValueError('Input string is longer than 100 characters')
    elif not re.match(r'^[a-z ]*$', text):
        raise ValueError('Input string contains characters other than lowercase a-z and space')

    # TODO determine range and alphabet info

    bottom = Decimal('0.0')
    top = Decimal('1.0')
    range = Decimal('1.0')

    alphabetInfo = getAlphabetInfo(text)
    fractions, totalLength = getAlphabetFractions(alphabetInfo)

    for c in text:
        bottom = bottom + range * fractions[c]['bottom']
        top = bottom + range * fractions[c]['top']
        range = top - bottom

    return (bottom, top, alphabetInfo)


def generateBinaryStringInRange(bottom, top):
    code = ''
    value = Decimal('0')
    while value < bottom:
        k = -(len(code)+1)
        bitVal = Decimal('2')**Decimal(k)
        if value + bitVal < top:
            value += bitVal
            code += '1'
        else:
            code += '0'
    return code


def getAlphabetFractions(alphabetInfo):
    rangeInfo = {}
    totalLength = 0

    for character, frequency in alphabetInfo:
        totalLength += frequency

    lastTop = Decimal('0')

    for character, frequency in alphabetInfo:
        newTop = lastTop + Decimal(frequency) / Decimal(totalLength)
        rangeInfo[character] = {
            'bottom': lastTop,
            'top': newTop,
        }
        lastTop = newTop
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

        with open(outputFilePath, 'w') as outputFile:
            outputFile.write(binaryString)
        
        with open(alphabetInfoFilePath, 'w') as alphabetInfoFile:
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
