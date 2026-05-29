#include <SDL2/SDL.h>
#include <stdbool.h>

#include "escrituraylectura.h"
#include "lista.h"
#include "masa.h"
#include "resorte.h"
#include "mallas.h"
#include "distancia.h"
#include "simulacion.h"


#include "config.h"

#include <SDL2/SDL_ttf.h>


void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s, int x, int y) {
    SDL_Color color = {255, 255, 255};  // Estaría bueno si la función recibe un enumerativo con el color, ¿no?
    SDL_Surface *surface = TTF_RenderText_Solid(font, s, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}


int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);


    TTF_Init();
    TTF_Font* font = TTF_OpenFont("FreeSansBold.ttf", 24);
    if(font == NULL) {
        fprintf(stderr, "No pudo cargarse la tipografía\n");
        return 1;
    }


    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Mono Bridge");

    int dormir = 0;

    // BEGIN código del alumno
    size_t nivel = 0;
    malla_t *malla = NULL;
    size_t id = 0;
    if(argc > 2){
        printf("Uso: %s %s\n", argv[0], "<nivel_#.bin>");
        return 1;
    }
    else if(argc == 2){
        FILE *entrada = fopen(argv[1], "rb");
        if(entrada == NULL){
            fprintf(stderr, "No se pudo abrir \"%s\"\n", argv[1]);
            return 1;
        }
        malla = leer_malla(entrada);
        if(malla == NULL){
            fclose(entrada);
            return 1;
        }
        
        id = malla_n_masas(malla);
        if(!leer_numero_de_datos(entrada, &nivel)){
            fclose(entrada);
            malla_destruir(malla);
            return 1;
        }
        fclose(entrada);
    }
    else {
        malla = crear_nivel(nivel, &id, (double)FACTOR_ESCALA);
        if(malla == NULL) return 1;
    }
    int coordx = 0, coordy = 0;
    int iniciox, inicioy;
    int finx, finy;
    double segundos = 0;
    masa_t *masaa;
    masa_t *masac;
    malla_simulacion_t *malla_s;
    simulacion_t *simulacion;
    
    bool conectando_masas = false;
    bool moviendo_masa = false;
    bool simulando = false;
    bool resorte_eliminado = false;
    bool ganando = false;
    // END código del alumno

    unsigned int ticks = SDL_GetTicks();
    while(1) {
        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;

            // BEGIN código del alumno
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT&& !simulando) {
                iniciox = event.motion.x;
                inicioy = event.motion.y;
                masaa = masa_buscar(malla, (double)iniciox/FACTOR_ESCALA, (double)inicioy/FACTOR_ESCALA, (double)EPSILON/FACTOR_ESCALA);
                if(masaa == NULL){
                    masaa = masa_crear((double)iniciox/FACTOR_ESCALA, (double)inicioy/FACTOR_ESCALA, false, id++, 0);
                    if(conectando_masas){
                        resorte_t *resorte = resorte_crear(masac, masaa, L0_MAX, 0);
                        conectando_masas = false;
                        if(resorte != NULL){
                            if(!malla_agregar_resorte(malla, resorte)){
                                malla_destruir(malla);
                                return 1;
                            }
                        }
                    }
                    if(masaa == NULL){
                        malla_destruir(malla);
                        return 1;
                    }
                    if(!malla_agregar_masa(malla, masaa)){
                        malla_destruir(malla);
                        return 1;
                    }
                } else if(conectando_masas && masaa != masac){
                    resorte_t *resorte = resorte_crear(masac, masaa, L0_MAX, 0);
                    conectando_masas = false;
                    if(resorte != NULL){
                        if(!malla_agregar_resorte(malla, resorte)){
                            malla_destruir(malla);
                            return 1;
                        }
                    }
                } else {
                    masac = masaa;
                    if(!masa_es_fijo(masac)){
                        moviendo_masa = true;
                    } 
                    else{
                        conectando_masas = true;
                    }
                }
            }
            else if(event.type == SDL_MOUSEMOTION && !simulando) {
                coordx = event.motion.x;
                coordy = event.motion.y;
                if(moviendo_masa){
                    double x_ant, y_ant;
                    masa_obtener_coords(&x_ant, &y_ant, masac);
                    masa_cambiar_coords(masac, (double)coordx/FACTOR_ESCALA, (double)coordy/FACTOR_ESCALA);
                    if(!malla_masa_actualizar_resortes_vinculados(malla, masac, L0_MAX, x_ant, y_ant)){
                        moviendo_masa = false;
                    }
                }
            }
            else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT &&!simulando) {
                finx = event.motion.x;
                finy = event.motion.y;
                if(moviendo_masa){
                    double distancia = distancia_xy((double)finx, (double)finy, (double)iniciox, (double)inicioy);
                    if(distancia < EPSILON){
                        conectando_masas = true;
                        moviendo_masa = false;
                    }
                }
                moviendo_masa = false;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT){
                if(simulando){
                    simulando = false;
                    malla_simulacion_destruir(malla_s);
                    simulacion_destruir(simulacion);
                    segundos = 0;
                }
                else{
                    int xborrar = event.motion.x;
                    int yborrar = event.motion.y;
                    resorte_eliminado = resorte_buscar_eliminar(malla, (double)xborrar/FACTOR_ESCALA, (double)yborrar/FACTOR_ESCALA, (double)EPSILON/FACTOR_ESCALA);
                    masa_t *mborrar = masa_buscar(malla, (double)xborrar/FACTOR_ESCALA, (double)yborrar/FACTOR_ESCALA, (double)EPSILON/FACTOR_ESCALA);
                    if(mborrar != NULL && !masa_es_fijo(mborrar)){
                        malla_sacar_masa(malla, mborrar);
                        id--;
                    }
                    if(mborrar == NULL && resorte_eliminado == false){
                        simulando = true;
                        size_t n_masas = malla_n_masas(malla);
                        malla_cambiar_masas_m_all(malla, (double)MASA_TOTAL/n_masas);
                        if(!malla_cambiar_resortes_k_lo_all(malla, L0_MAX, (double)K_BASE, (double)POTENCIA_K)){
                            malla_destruir(malla);
                            return 1;
                        }
                        malla_s = malla_simulacion_crear(malla);
                        if(malla_s == NULL){
                            malla_destruir(malla);
                            return 1;
                        }
                        simulacion = simulacion_crear(malla_s, G, B, DT);
                        if(simulacion == NULL){
                            malla_destruir(malla);
                            malla_simulacion_destruir(malla_s);
                            return 1;
                        }
                    }
                }
            }
            // END código del alumno

            continue;
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);

        // BEGIN código del alumno
        if(simulando){
            conectando_masas = false;
            if(segundos < DURACION_SIMULACION){
                escribir_texto(renderer, font, "PROCESANDO", 100, 20);
            }
            else if(segundos >= DURACION_SIMULACION && ganando){
                escribir_texto(renderer, font, "FELICITACIONES, GANASTE", 100, 20);
                char nombre[20];
                sprintf(nombre, "nivel_%zd.bin", nivel + 1);
                FILE *salida = fopen(nombre, "wb");
                if(salida == NULL || !escribir_malla(malla, salida, nivel)){
                    fprintf(stderr, "No se guardo la solucion al nivel");
                    if(salida != NULL){
                        fclose(salida);
                    }
                }
                else {
                    fclose(salida);
                }
                if(segundos > DURACION_SIMULACION + 3){
                    simulando = false;
                    malla_simulacion_destruir(malla_s);
                    simulacion_destruir(simulacion);
                    malla_destruir(malla);
                    nivel++;
                    segundos = 0;
                    id = 0;
                    malla = crear_nivel(nivel, &id, (double)FACTOR_ESCALA);
                }
            }
            else if(segundos >= DURACION_SIMULACION && !ganando){
                escribir_texto(renderer, font, "PERDISTE", 100, 20);
                if(segundos > DURACION_SIMULACION + 5){
                    simulando = false;
                    malla_simulacion_destruir(malla_s);
                    simulacion_destruir(simulacion);
                    segundos = 0;
                }
            }
        }
        else {
            escribir_texto(renderer, font, "Mono Bridge", 100, 20);
            char aux2[100];
            sprintf(aux2, "NIVEL %zd", nivel + 1);
            escribir_texto(renderer, font, aux2, 300, 20);
        }
        
        char aux[100];
        sprintf(aux, "%03d, %03d", coordx, coordy);
        escribir_texto(renderer, font, aux, VENTANA_ANCHO - 100, VENTANA_ALTO - 34);
        
        
        
        if(conectando_masas && !simulando){
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
            double xaux, yaux;
            masa_obtener_coords(&xaux, &yaux, masac);
            xaux *= FACTOR_ESCALA;
            yaux *= FACTOR_ESCALA;
            double lo = distancia_xy(xaux, yaux, (double)coordx,(double) coordy);
            if(lo <= L0_MAX * FACTOR_ESCALA){
                SDL_RenderDrawLine(renderer, xaux, yaux, coordx, coordy);
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
                float ancho = 10;
                SDL_Rect r1 = {coordx - ancho/2, coordy - ancho/2, ancho, ancho};
                SDL_RenderDrawRect(renderer, &r1);
            }
        }
        if(malla != NULL && !simulando){
            float ancho = 10;
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
            mallas_iter_masa_t *mm;
            for(mm = mallas_iter_masa_crear(malla); mm != NULL && !mallas_iter_masa_al_final(mm); mallas_iter_masa_avanzar(mm)){
                double x, y;
                masa_t *masa = mallas_iter_ver_masa_actual(mm);
                masa_obtener_coords(&x, &y, masa);
                x *= FACTOR_ESCALA;
                y *= FACTOR_ESCALA;
                SDL_Rect r2 = {x - ancho/2, y - ancho/2, ancho, ancho};
                SDL_RenderDrawRect(renderer, &r2);
            }
            if(mm == NULL){
                malla_destruir(malla);
                return 1;
            }
            mallas_iter_masa_destruir(mm);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
            mallas_iter_resorte_t *mi;
            for(mi = mallas_iter_resorte_crear(malla); mi != NULL && !mallas_iter_resorte_al_final(mi); mallas_iter_resorte_avanzar(mi)){
                resorte_t *resorte = mallas_iter_ver_resorte_actual(mi);
                double x1, y1, x2, y2;
                resorte_obtener_coords(&x1, &y1, &x2, &y2, resorte);
                x1 *= FACTOR_ESCALA;
                y1 *= FACTOR_ESCALA;
                x2 *= FACTOR_ESCALA;
                y2 *= FACTOR_ESCALA;
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
            if(mi == NULL){
                malla_destruir(malla);
                return 1;
            }
            mallas_iter_resorte_destruir(mi);
        }
        if(simulando){
            if(!simular(simulacion, ((double)1 / JUEGO_FPS)/ DT)){
                malla_destruir(malla);
                malla_simulacion_destruir(malla_s);
                simulacion_destruir(simulacion);
                return 1;
            }
            else{
                segundos += (double)1/JUEGO_FPS;
            }
            char aux[100];
            sprintf(aux, "%.2f", segundos);
            escribir_texto(renderer, font, aux, VENTANA_ANCHO - 100, 34);
            float ancho = 10;
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
            mallas_s_iter_masa_t *mm;
            for(mm = mallas_s_iter_masa_crear(malla_s); mm != NULL && !mallas_s_iter_masa_al_final(mm); mallas_s_iter_masa_avanzar(mm)){
                double x, y;
                masa_t *masa = mallas_s_iter_ver_masa_actual(mm);
                masa_obtener_coords(&x, &y, masa);
                x *= FACTOR_ESCALA;
                y *= FACTOR_ESCALA;
                SDL_Rect r2 = {x - ancho/2, y - ancho/2, ancho, ancho};
                SDL_RenderDrawRect(renderer, &r2);
            }
            if(mm == NULL){
                malla_destruir(malla);
                return 1;
            }
            mallas_s_iter_masa_destruir(mm);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
            mallas_s_iter_resorte_t *mi;
            for(mi = mallas_s_iter_resorte_crear(malla_s); mi != NULL && !mallas_s_iter_resorte_al_final(mi); mallas_s_iter_resorte_avanzar(mi)){      //
                resorte_t *resorte = mallas_s_iter_ver_resorte_actual(mi);
                double x1, y1, x2, y2;
                double lo = resorte_obtener_lo(resorte);
                resorte_obtener_coords(&x1, &y1, &x2, &y2, resorte);
                double lr = distancia_xy(x1, y1, x2, y2);
                x1 *= FACTOR_ESCALA;
                y1 *= FACTOR_ESCALA;
                x2 *= FACTOR_ESCALA;
                y2 *= FACTOR_ESCALA;
                if((lr - lo)/lo > MAXIMO_ESTIRAMIENTO){
                    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
                }
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
            if(mi == NULL){
                malla_destruir(malla);
                return 1;
            }
            mallas_s_iter_resorte_destruir(mi);
            if(segundos > DURACION_SIMULACION && segundos < DURACION_SIMULACION + 0.1){
                for(mi = mallas_s_iter_resorte_crear(malla_s); mi != NULL && !mallas_s_iter_resorte_al_final(mi); mallas_s_iter_resorte_avanzar(mi)){      //
                    resorte_t *resorte = mallas_s_iter_ver_resorte_actual(mi);
                    double x1, y1, x2, y2;
                    double lo = resorte_obtener_lo(resorte);
                    resorte_obtener_coords(&x1, &y1, &x2, &y2, resorte);
                    double lr = distancia_xy(x1, y1, x2, y2);
                    if((lr - lo)/lo > MAXIMO_ESTIRAMIENTO){
                        ganando = false;
                        break;
                    }
                    ganando = true;
                }
            if(mi == NULL){
                malla_destruir(malla);
                return 1;
            }
            mallas_s_iter_resorte_destruir(mi);
            }
        }
        // END código del alumno

        SDL_RenderPresent(renderer);
        ticks = SDL_GetTicks() - ticks;
        if(dormir) {
            SDL_Delay(dormir);
            dormir = 0;
        }
        else if(ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        ticks = SDL_GetTicks();
    }

    // BEGIN código del alumno
    malla_destruir(malla);
    if(simulando){
        malla_simulacion_destruir(malla_s);
        simulacion_destruir(simulacion);
    }
    // END código del alumno

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    TTF_CloseFont(font);
    TTF_Quit();

    SDL_Quit();
    return 0;
}
