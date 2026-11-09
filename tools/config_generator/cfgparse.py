"""Minimal parser for derapified Arma configs (HEMTT output). Merges several files."""
import re, sys, json, glob, os

TOKEN = re.compile(r'''
    (?P<ws>\s+)|
    (?P<comment>//[^\n]*|/\*.*?\*/)|
    (?P<str>"(?:[^"]|"")*")|
    (?P<sym>[{}\[\];:=,])|
    (?P<word>[^\s{}\[\];:=,"]+)
''', re.X | re.S)

def tokens(text):
    for m in TOKEN.finditer(text):
        k = m.lastgroup
        if k in ('ws', 'comment'):
            continue
        v = m.group()
        if k == 'str':
            v = ('S', v[1:-1].replace('""', '"'))
        yield v

class P:
    def __init__(self, text):
        self.t = list(tokens(text)); self.i = 0
    def peek(self): return self.t[self.i] if self.i < len(self.t) else None
    def nxt(self): v = self.t[self.i]; self.i += 1; return v
    def body(self, node):
        while self.peek() is not None and self.peek() != '}':
            tok = self.nxt()
            if tok == ';': continue
            if tok == 'class':
                name = self.nxt()
                parent = None
                if self.peek() == ':':
                    self.nxt(); parent = self.nxt()
                if self.peek() == ';':
                    self.nxt(); node['_c'].setdefault(name.lower(), {'_n': name, '_p': parent, '_c': {}, '_v': {}, '_ext': True}); continue
                assert self.nxt() == '{'
                child = node['_c'].get(name.lower())
                if child is None or child.get('_ext'):
                    child = {'_n': name, '_p': parent, '_c': {}, '_v': {}}
                    node['_c'][name.lower()] = child
                elif parent: child['_p'] = parent
                self.body(child)
                assert self.nxt() == '}'
                continue
            if tok in ('delete',):
                self.nxt(); continue
            key = tok
            arr = False
            if self.peek() == '[':
                self.nxt(); self.nxt(); arr = True
            op = self.nxt()
            if op == '+': op = self.nxt()
            assert op == '=', (key, op)
            val = self.array() if arr else self.scalar()
            node['_v'][key.lower()] = val
    def scalar(self):
        parts = []
        while self.peek() not in (';', '}', None):
            v = self.nxt(); parts.append(v[1] if isinstance(v, tuple) else v)
        return parts[0] if len(parts) == 1 else ' '.join(parts)
    def array(self):
        assert self.nxt() == '{'
        out = []
        while self.peek() != '}':
            if self.peek() == ',': self.nxt(); continue
            if self.peek() == '{': out.append(self.array()); continue
            v = self.nxt(); out.append(v[1] if isinstance(v, tuple) else v)
        self.nxt()
        return out

def load(files):
    root = {'_n': '', '_p': None, '_c': {}, '_v': {}}
    for f in files:
        with open(f, encoding='utf-8', errors='ignore') as fh:
            P(fh.read()).body(root)
    return root

def get(cls_section, name, key, depth=0):
    """Value with inheritance inside one section (CfgMagazines etc.)."""
    node = cls_section['_c'].get(name.lower())
    while node is not None and depth < 50:
        if key in node['_v']: return node['_v'][key]
        p = node['_p']
        node = cls_section['_c'].get(p.lower()) if p else None
        depth += 1
    return None
