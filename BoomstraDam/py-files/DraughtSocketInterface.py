#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#



import select
import socket
import itertools
from DraughtEngineInterface import *
from DraughtRemotePlayer import *
import os
import errno
import Tkinter

# Algumas constantes usadas

INPUT_READ = 0
INPUT_WRITE = 1
INPUT_EXCEPTION = 2

_read  = {}
_write = {}
_error = {}

_modes = [_read, _write, _error]
_select = select.select

_tags = itertools.count()

class _Wrapper(object):
    # Classe wrapper para os sockets ou arquivos usados

    def __init__(self, source, callback):
        self.source = source
        self.fileno = source.fileno
        self.close = source.close
        self.callback = callback
        self.s = ''
        self.recv = source.recv


class TkSelectMixIn:
    # Classe wrapper para select.select().
    def __init__(self, root):
        self.root = root
        return

    def input_add(self, source, mode, callback):
        print "input_add"
        # Insere um objeto na pilha e retorna um identificador
        #
        # source  o objeto, normalmente um socket
        #
        # mode e uma das constantes INPUT_READ, INPUT_WRITE ou
        # INPUT_EXCEPTION
        #
        # callback e o metodo a ser chamado quando a condicao
        # monitorada for satisfeita

        if mode not in (0, 1, 2):
            raise ValueError("invalid mode")

        tag = _tags.next()
        _modes[mode][tag] = _Wrapper(source, callback)

        return tag

    def input_remove(self, tag):
        # Remove um objeto da pilha. key e o identificador retornado
        # quando o objeto foi inserido
        # Note que o socket NAO E FECHADO, apenas removido

        for mode in _modes:
            if tag in mode:
                mode.pop(tag)
                break

    def _check(self):
        # Verifica todos os sockets sendo usados e remove quaisquer
        # que estejam com problemas

        for mode in _modes:
            for tag, source in mode.iteritems():
                try:
                    _select([source], [source], [source], 0)
                except: # encontramos o vilao
                    mode.remove(tag)
                    source.close()

    def _select(self,
              # Essa declaracao estranha tem uma finalidade. Armazenar
              # as globais no namespace local, acelerando a consulta
              # de nomes, permitindo que este metodo seja executado o
              # mais rapido possivel.
              _read=_read,
              _write=_write,
              _error=_error,
              _select=_select):
        while 1:

            # tentamos o select ate nao ocorrer erros
            try:
                if len(_read.values()) + len( _write.values()) + len(_error.values()) > 0:
                    ready = _select(_read.values(), _write.values(), _error.values(), 0)
                else:
                    ready = [], [], []
                break
            except ValueError:
                # Um socket invalido foi passado... 
                self._check()
            except TypeError:
                # Algo que nao era um socket foi passado...
                self._check()
            except socket.error, reason:
                # Algum dos sockets esta com problemas... 
                code, msg = reason.args
                if code == errno.EINTR:
                    print os.strerror(errno.EINTR)
                    # consulte man 2 select
                    return
                if code == errno.EBADF:
                    print os.strerror(errno.EBADF)
                    # Socket com problemas... 
                    self._check()
                else:
                    # Se chegamos aqui, realmente nao sei o que ocorreu
                    raise

        for condition, mode in enumerate(ready):
            for source in mode:
                data = source.recv(128)
                if len(data) == 0:
                    source.close()
                    source.callback('X')    # tell them we are closed

                    for addr, (conn, tag) in self.clients.iteritems():
                        if source is conn:
                            self.input_remove(tag)
                            del self.clients[addr]
                            break
                else:
                    for i in range(0, len(data)):
                        if data[i] == '\0':
                            source.callback(source.s)
                            source.s = ''
                        else:
                            source.s = source.s + data[i]


        self.root.after(100, self._select)


    def start(self):
        self.root.after(2000, self._select)


class SocketInterface(TkSelectMixIn):
    def __init__(self, root = None):
        if root == None:
            root = Tk()
        self.root = root
        # startup socket listening for remote player and engine
        self.socmon = TkSelectMixIn(self.root)
        self.socmon.clients = {}
        self.socmon.start()
        return
    

if __name__=='__main__':
    root = Tk()
    test = SocketInterface(root)
    root.mainloop()
        