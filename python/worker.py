# -*- coding: utf-8 -*-
import hashlib
import xml.etree.cElementTree as ET
import argparse
import time

from lxml import etree


def pseudonimize(string):
    dot = string.find('.')
    tohash = string[dot+1:]
    l = len(tohash)
    suffix = hashlib.sha512(tohash).hexdigest()[:l]
    return string[:dot+1] + suffix


def process_lxml(fname):
    start = time.time()
    tree = etree.parse(fname)
    for elem in tree.findall("/item/field[@name='origin']"):
        nn = pseudonimize(elem.text)
        elem.text = nn
    elapsed = time.time() - start
    with open('%s.%s' % (fname, 'python.lxml.anon'), 'wb') as f:
        tree.write(f, pretty_print=True)
    return elapsed


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', default='/dev/stdin',
                        help='Input xml file. Default /dev/stdin')
    return parser.parse_args()


if __name__ == '__main__':
    args = parse_args()

    process = process_lxml
    start = time.time()
    ptime = process(args.input)
    elapsed = time.time() - start

    to_milli_seconds = lambda t: int(round(t * 1000))

    print '%d\t%d' % (to_milli_seconds(ptime), to_milli_seconds(elapsed))
