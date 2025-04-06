def add_escape_characters(input_string):
    special_chars = [' ', '(', ')', '^', '"', '\'', '~', '*', '?', ':', '\\']
    result = ""
    for char in input_string:
        if char in special_chars:
            result += '\\' + char
        else:
            result += char
    return str(result)