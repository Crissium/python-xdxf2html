# Description

This is a Python module that converts XDXF formatted dictionary texts into HTML, written in modern C++.

It depends on nothing except for a C++11-compliant compiler. The parser does no error checking to minimise overhead.

# Installation

```bash
pip install xdxf2html
```

# Building

```bash
python3 setup.py build
```

# Usage

```python
>>> import xdxf2html
>>> xdxf2html.convert('''<k>Liverpool</k>
... <blockquote><dtrn>a large city and port in north-west England, on the River Mersey. It first became important during the <kref>Industrial Revolution</kref>, producing and exporting cotton goods. It was also a major port for the slave trade, receiving profits from the sale of slaves in America. In the 20th century the city became famous as the home of the <kref>Beatles</kref> and for Liverpool and Everton football clubs. Among its many famous buildings are the Royal Liver Building with its two towers, the Anglican and Roman Catholic cathedrals, and the <kref>Walker Art Gallery</kref>.</dtrn> <rref>portlpool.jpg</rref></blockquote>
... <blockquote>See also <kref>Mersey beat</kref>.</blockquote>''', '/static', '/lookup')
'<h3 class="headword">Liverpool</h3><div class="xdxf-definition" style="margin-left: 0em;">a large city and port in north-west England, on the River Mersey. It first became important during the <a href="/api/lookup/test_dict/Industrial Revolution">Industrial Revolution</a>, producing and exporting cotton goods. It was also a major port for the slave trade, receiving profits from the sale of slaves in America. In the 20th century the city became famous as the home of the <a href="/api/lookup/test_dict/Beatles">Beatles</a>and for Liverpool and Everton football clubs. Among its many famous buildings are the Royal Liver Building with its two towers, the Anglican and Roman Catholic cathedrals, and the <a href="/api/lookup/test_dict/Walker Art Gallery">Walker Art Gallery</a>.<img src="/api/cache/test_dict/portlpool.jpg" alt="portlpool.jpg"/></div><div class="xdxf-definition" style="margin-left: 0em;">See also <a href="/api/lookup/test_dict/Mersey beat">Mersey beat</a>.</div>'
```
The module has only one method: `convert`, which takes three arguments: the XDXF text and the base URLs for static files and lookup. It returns the HTML text.

# Appendix: a hopefully complete listing of XDXF tags, both standard and non-standard

This section will only include tags found within the dictionary 'body', i.e. `<lexicon>`.

## Representational tags

- `<b>`, `<i>`, `<u>`, `<sub>`, `<sup>`, `<tt>`, `<br>`: Could be directly translated into HTML.
- `<c>`: Colour, indicated by the attribute `c`. Converted to a `<span>` with the style `color: c`. The default colour is `darkgreen`.

## `<div>` like tags

- `<ar>`: Article, ignored in this project.
- `<def>`: Definition, converted to a `<div>` with the class `xdxf-definition`. If the attribute `cmt` is present, write it as a `<span>` with the class `comment` inside the `<div>`; if the attribute `freq` is present, write it as a `<span>` with the class `frequency` inside the `<div>`.
- `<ex>`: Example, converted to a `<div>` with the class `example` and the style `margin-left: 1em; color: grey;`.
- `<co>`: Comment, converted to a `<div>` with the class `comment`.
- `<sr>`: 'Semantic relations', converted to a `<div>` with the class `semantic-relations`.
- `<etm>`: Etymology, converted to a `<div>` with the style `color: grey;`.
- `<blockquote>`: Not in the specification, but I've seen it. Converted to a `<p>`.

## `<span>` like tags

- `<k>`: Keyword, namely the headword, converted to an `<h3>` with the class `headword`.
- `<opt>`: Optional part of the keyword, converted to a `<span>` with the class `optional`.
- `<deftext>`: Definition text, ignored in this project.
- `<gr>`: Grammatical information, converted to a `<span>` with the style `font-style: italic; color: darkgreen;`.
- `<pos>`, `<tense>`: Ignored.
- `<tr>`: Transcription, converted to a `<span>` with the class `transcription`.
- `<kref>`: Keyword reference, converted to an `<a>` with the `href` attribute properly set (in this project, to `/api/lookup/name of dictionary/keyword`). If the attribute `type` is set, prepend the value of the attribute to the keyword with a colon and a space.
- `<iref>`: External reference, could be directly converted to an `<a>`.
- `<dtrn>`: Definition translation, in practice used as an equivalent of `<def>`. So we'd better ignore it.
- `<abbr>`, `<abr>`: Abbreviation, converted to a `<span>` with the class `abbreviation` and the style `color: darkgreen; font-style: italic;`.
- `<ex_orig>`, `<ex_trn>`: Example original and translation, converted to a `<span>` with the class `example-original` and `example-translation` respectively.
- `<exm>`, `<prv>`, `<oth>`: Used inside `<ex>`, ignored in this project.
- `<mrkd>`: Marked text, converted to a `<span>` with the style `background-color: yellow;`.
- `<nu>`: Not-used, not explained in the specification. Ignored in this project.
- `<di>`: 'Don't index', ignored in this project.
- `<syn>, <ant>, <hpr>, <hpn>, <par>, <spv>, <mer>, <hol>, <ent>, <rel>, <phr>`: Phrasemes (don't know really what they are). `<syn>` and `<ant>` are converted to `<span>` with the class `synonym` and `antonym` respectively, and the tag name is prepended to the text content with `: `. The rest are ignored.
- `<categ>`: Category, ignored in this project.

## Media files

Always in an `<rref>` tag. The standard prescribes that the filename should be specified in the `lctn` attribute, but in practice, the filename is just the text child of the tag. Converted to `<img>`, `<audio>`, `<video>` or `<a>` depending on the file extension.
