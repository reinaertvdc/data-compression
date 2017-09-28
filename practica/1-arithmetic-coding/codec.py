import decimal
from decimal import Decimal
import json
import re
import sys


# Set the precision of the decimal package to 200
decimal.getcontext().prec = 200


def binStringToDecimalFraction(binStr):
    """
    Converts a string (str object) consisting of the characters '0' and '1' to its decimal representation
    :param binStr: the binary string
    :return: Decimal() object containing the decimal representation of binStr
    """
    # Keep track of the current sum (starts at 0)
    sum = Decimal('0.0')
    # Loop over characters in the binary string
    for i, c in enumerate(binStr):
        # If the character is a '1' and not '0': add the value of the current position to the sum
        if c == '1':
            sum += Decimal('2')**Decimal(str(-(i+1)))
    # Return the total sum
    return sum


def decodeDecimalFraction(value, alphabetInfo):
    """
    Decodes a decimal value to a string based on info about an alphabet
    :param value: The value to decode as a Decimal() object
    :param alphabetInfo: The info about the alphabet (including raw frequencies)
    :return: The decoded string
    """
    # Convert the alphabet raw frequencies into fractions
    fractions, totalLength = getAlphabetFractions(alphabetInfo)
    # Keep track of the decoded part of the string
    s = ''

    bottom = Decimal('0')
    top = Decimal('1')

    # Loop until the string has been decoded completely
    while len(s) < totalLength:
        # Find the fraction interval in which the value is contained
        for key, val in fractions.items():
            # Check the value against the range
            bottom = val['bottom']
            top = val['top']
            if bottom <= value < top:
                # In range, then add the character to the string
                s += key
                break
        # Zoom in onto the interval, this means updating the value based on the new range
        range = top - bottom
        value = (value - bottom) / range
    # Return the decoded string
    return s


def getAlphabetInfo(text):
    """
    Calculate info about the alphabet (including raw frequencies) based on a string
    :param text: The text to use while building the alphabet info
    :return: An array containing information about the alphabet.
             Each element is an array of the structure [character, rawFrequency]
             The outer level array is sorted on the character of the nested array
    """
    # Build the information using a dictionary
    result = dict()
    for c in text:
        if c in result:
            result[c] += 1
        else:
            result[c] = 1
    # Convert the dictionary to an array (sorted on the key value of the dictionary)
    out = []
    for c in sorted(result):
        out.append([c, result[c]])
    # Return the alphabet info array
    return out


def encodeToRange(text):
    """
    Encode a string to a range within the value should be
    :param text: the string to encode
    :return: a list consisting of the range and the alphabet info: (bottom, top, alphabetInfo)
    """
    # Check the input string
    if len(text) > 100:
        raise ValueError('Input string is longer than 100 characters')
    elif not re.match(r'^[a-z ]*$', text):
        raise ValueError('Input string contains characters other than lowercase a-z and space')
    # Keep track of the current range
    bottom = Decimal('0.0')
    top = Decimal('1.0')
    range = top - bottom
    # Get information about the alphabet (raw and normalized frequencies)
    alphabetInfo = getAlphabetInfo(text)
    fractions, totalLength = getAlphabetFractions(alphabetInfo)
    # Loop the characters in the string
    for c in text:
        # Based on the current character, recalculate the range
        top = bottom + range * fractions[c]['top']
        bottom = bottom + range * fractions[c]['bottom']
        range = top - bottom
    # Return the range and the alphabet info
    return (bottom, top, alphabetInfo)


def generateBinaryStringInRange(bottom, top):
    """
    Generates the shortest possible binary string (string of '0' and '1') representing a value within a given range
    :param bottom: the bottom of the range
    :param top: the top of the range
    :return: The binary string representation of a value within the range (as a python str object)
    """
    # Keep track of the current binary string encoding, together with it's decimal value
    code = ''
    value = Decimal('0')
    # While the value is not yet in the range (value will never go over the range within the loop)
    while value < bottom:
        # calculate the value of the next bit that is added
        k = -(len(code)+1)
        bitVal = Decimal('2')**Decimal(k)
        # Check if the bit should be added ('1') or not ('0') and append it
        if value + bitVal < top:
            value += bitVal
            code += '1'
        else:
            code += '0'
    # Return the bit string
    return code


def getAlphabetFractions(alphabetInfo):
    """
    Get the fractions of character frequencies based on information of an alphabet
    :param alphabetInfo: Information about an alphabet consisting of raw frequencies of symbols as returned by getAlphabetInfo()
    :return: A tuple consisting of a dict containing the fractions and the total length of the string (fractionsDict, totalLength)
             The fractions dictionary keys are the symbols of the alphabet, the values are dictionaries again.
             Those last dictionaries consist of 2 key-value pairs {'top': top of the range, 'bottom': bottom of the range}
             All range values are within 0 and 1
    """
    # Keep track of the current fractions and length
    rangeInfo = {}
    totalLength = 0
    # Calculate the total length of the string based on the alphabet info
    for character, frequency in alphabetInfo:
        totalLength += frequency
    # Temporary variable
    lastTop = Decimal('0')
    # Loop the alphabet info characters and frequencies
    for character, frequency in alphabetInfo:
        # Calculate the top of the fraction range of the current character
        newTop = lastTop + Decimal(frequency) / Decimal(totalLength)
        # Add the info about the current character to the fractions dictionary
        rangeInfo[character] = {
            'bottom': lastTop,
            'top': newTop,
        }
        # Remember the top value for the next iteration
        lastTop = newTop
    # Return the correct values
    return (rangeInfo, totalLength)


if __name__ == '__main__':
    # Mode (encode, decode, test)
    mode = sys.argv[1]
    # Get the input file path
    inputFilePath = sys.argv[2]
    # Read the input file and remove line endings
    with open(inputFilePath) as inputFile:
        inputString = inputFile.read().replace('\r\n', '').replace('\n', '')
    # TEST mode
    if mode == 'test':
        # Calculate the range, get the mean value between it and decode again
        bottom, top, alphabetInfo = encodeToRange(inputString)
        mean = (bottom + top) / 2
        outputString = decodeDecimalFraction(mean, alphabetInfo)
        # Output to console
        print(' INPUT: "' + inputString + '"')
        print('OUTPUT: "' + outputString + '"')
        print('They are' + ('' if inputString == outputString else ' NOT') + ' identical.')
    # ENCODE mode
    elif mode == 'encode':
        # read encoded string output file and alphabet info output file from command line arguments
        outputFilePath = sys.argv[3]
        alphabetInfoFilePath = sys.argv[4]
        # Calculate the binary string value
        bottom, top, alphabetInfo = encodeToRange(inputString)
        binaryString = generateBinaryStringInRange(bottom, top)
        # Write output
        with open(outputFilePath, 'w') as outputFile:
            outputFile.write(binaryString)
        with open(alphabetInfoFilePath, 'w') as alphabetInfoFile:
            json.dump(alphabetInfo, alphabetInfoFile)
    # DECODE mode
    elif mode == 'decode':
        # Read decoded string output file and alphabet info input file from command line arguments
        alphabetInfoFilePath = sys.argv[3]
        outputFilePath = sys.argv[4]
        # Read alphabet info from specified file
        with open(alphabetInfoFilePath) as alphabetInfoFile:
            alphabetInfo = json.load(alphabetInfoFile)
        # Decode the binary string into a string
        fraction = binStringToDecimalFraction(inputString)
        outputString = decodeDecimalFraction(fraction, alphabetInfo)
        # Write the decoded string to a file
        with open(outputFilePath, 'w') as outputFile:
            outputFile.write(outputString)
