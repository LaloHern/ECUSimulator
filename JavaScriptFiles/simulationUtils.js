function map(input, in_min, in_max, out_min, out_max) {
    var output = (input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

    return output
}
