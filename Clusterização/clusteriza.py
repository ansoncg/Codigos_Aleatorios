import sys
import ctypes
import numpy as np
from sklearn.cluster import DBSCAN, KMeans
from sklearn.preprocessing import StandardScaler
from sdl2 import *


size = 500


class sdl_vars:

    def __init__(self):
        self.delay = 1000 / 150
        self.Tini = 0
        self.Tfim = 0
        self.salvar_img = False
        self.mousePos = np.array([0, 0])
        self.mouseApertado = False
        self.rodando = True
        self.evento = SDL_Event()
        self.db = False
        self.pontos = np.empty((size, size))
        SDL_Init(SDL_INIT_EVERYTHING)
        self.janela = SDL_CreateWindow(b"Figuras", 0, 0, size, size, False)
        self.renderizador = SDL_CreateRenderer(self.janela, -1, 0)
        SDL_SetRenderDrawColor(self.renderizador, 255, 255, 255, 255)
        SDL_RenderClear(self.renderizador)

    def classificar(self):
        pontos_classifica = np.empty((0, 2))
        for i in range(size):
            for j in range(size):
                if self.pontos[i][j] == 1:
                    pontos_classifica = np.concatenate((pontos_classifica, [[i, j]]), axis=0)
        scaler = StandardScaler()
        features = scaler.fit_transform(pontos_classifica)

        if (self.db):
            cluster = DBSCAN(eps=0.05, min_samples=10)
        else:
            cluster = KMeans(n_clusters=2)  # Dois clusters
        cluster.fit(features)
        previsoes = cluster.labels_

        SDL_SetRenderDrawColor(self.renderizador, 255, 255, 255, 255)
        SDL_RenderClear(self.renderizador)
        for i in range(len(pontos_classifica)):
            r = (previsoes[i] * 100 + 100) % 255
            g = (previsoes[i] * 100 - 100) % 255
            b = (previsoes[i] * 100 * 5) % 255
            SDL_SetRenderDrawColor(self.renderizador, r, g, b, 255)
            SDL_RenderDrawPoint(self.renderizador, int(pontos_classifica[i][1]) + 3, int(pontos_classifica[i][0]) + 2)

    def destroi(self):
        SDL_DestroyWindow(self.janela)
        SDL_DestroyRenderer(self.renderizador)
        SDL_Quit()

    def eventos(self):
        while SDL_PollEvent(ctypes.byref(self.evento)) != 0:
            if self.evento.type == SDL_QUIT:
                self.rodando = False
                break
            elif self.evento.type == SDL_MOUSEMOTION:
                self.mousePos[0] = self.evento.motion.x
                self.mousePos[1] = self.evento.motion.y
                break
            elif self.evento.type == SDL_MOUSEBUTTONDOWN:
                self.mouseApertado = True
                break
            elif self.evento.type == SDL_MOUSEBUTTONUP:
                self.mouseApertado = False
                break
            elif self.evento.type == SDL_KEYDOWN:
                estados = SDL_GetKeyboardState(None)
                if (estados[SDL_SCANCODE_D]):
                    self.salvar_img = True
                    self.db = True
                elif (estados[SDL_SCANCODE_K]):
                    self.salvar_img = True
                    self.db = False
                elif (estados[SDL_SCANCODE_C]):
                    self.pontos = np.empty((size, size))
                    SDL_SetRenderDrawColor(self.renderizador, 255, 255, 255, 255)
                    SDL_RenderClear(self.renderizador)

    def desenha(self):
        if (self.mouseApertado):
            SDL_SetRenderDrawColor(self.renderizador, 0, 0, 0, 255)
            for i in range(7):
                for j in range(7):
                    self.pontos[self.mousePos[1] + j][self.mousePos[0] + i] = 1
                    SDL_RenderDrawPoint(self.renderizador, self.mousePos[0] + i, self.mousePos[1] + j)
        SDL_RenderPresent(self.renderizador)
        if (self.salvar_img):
            self.classificar()
            self.salvar_img = False


def main():
    sdlvars = sdl_vars()

    while sdlvars.rodando:
        sdlvars.Tini = SDL_GetTicks()
        sdlvars.eventos()
        sdlvars.desenha()
        sdlvars.Tfim = SDL_GetTicks() - sdlvars.Tini
        if sdlvars.Tfim < sdlvars.delay:
            SDL_Delay(int(sdlvars.delay - sdlvars.Tfim))

    sdlvars.destroi()
    return 0


if __name__ == "__main__":
    sys.exit(main())
