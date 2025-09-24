# Saleae PDM Analyzer
This analyzer is useful for PDM streams. It counts the number of 1s present on
the data line for a given clock's rising edges. In PDM terms, that means it only
does the first channel. It doesn't handle stereo currently.

It does its best to start with the clock signal. However, it may be shifted
against the samples you receive in your code. Changing the bits per sample
configuration sample to 1 can make it easy to read individual bits instead of the
ones count for a set of bits.

# Using

### Compiling on Windows

Use the Visual Studio project to compile.

### Compiling on Mac OSX/Linux

```bash
python build_analyzer.py
```

## Including in Saleae Logic
Set the search path for plugins to the `release` directory within your clone of
this repo.

To find the setting:

1. Open the **Options** drop down in the top right.
2. Select **Preferences**
3. Select the **Developer** tab.
4. The analyzer path is first in the pane.

Now, reload Logic and select **PDM** from the Analyzer drop down.

Please see the [Sample Analyzer](https://github.com/saleae/SampleAnalyzer/blob/master/readme.md) 
for detailed build instructions.

## Exporting as WAV audio files & logic2

Not for Logic2 -- the ability for Analysers to export to (custom) files was somehow broken/lost 
in the Logic 1 to 2 transition.

As a work around - this version allows you to select the file export format in settings (so not 
during the export itself). You then use the (now only) available `export as CSV/text' option
which will, in fact, save the file as the format specified (e.g. WAV or raw PCM).
