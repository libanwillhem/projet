#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "piece.h"
#include "interface.h"

//=NB JOUEURS,JOUEUR TOTAUX=====//
int J= 50 ; //nb de joueur total
int J_HUMAIN= 1 ;//nb de joueur humain parmis les joueurs totales
//==============================//

//==UNITEES=====================//
int NB_CLASSE= 6 ; //nb de classe actuelement dans le jeu !!!!!A ne pas modifier!!!!!!
int NB_UNITE= 30 ; //nb unité pour chaque joueurs au debut de la partie
//==============================//

//===========VITESSE DU JEU=====//
int PTS_ACTION_MAX= 20 ; //pts d'action max pour chaque tours de chaque joueur
int VITESSE_JEU_BOT= 1 ; //nb de boucle d'affichage entre chaque action d'un bot (vitesse max=1)
int VITESSE_ANIM=25;
//==============================//

//========AFFICHAGE/GRILLE======//
int AFF_DEG= 10 ; //nombre d'affichage max a la fois par boucle d'affichage d'info texte de dégats, morts et soins
int PRESET= 1 ; //1 pour generation alea, autre pour preset de carte via fichier

int N= 100 ; //taille de la grille (ne peux pas eccéder 200x200 actuelement (mettre en place des fichier ou enregistrer et reouvrir pour chargement dynamique de la map et grandeur infini))
int M= 100 ;
//==============================//


piece_t * init_piece(classe_t classe,int id_joueur){
    int pos=((rand()%(4)) + 1);
    piece_t * piece=malloc(sizeof(piece_t));
    int frame=((rand()%2)+1);
    int frame2=((rand()%10)+1);
    switch (classe){
        case knight:
            piece->classe = knight;
            piece->pdv=35;
            piece->puissance = 22;
            piece->armure=10;
            piece->block=80;
            piece->portee=1;
            piece->deplacement=2;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame;
            piece->frame_interface=frame;
        break;
        case scout:
            piece->classe = scout;
            piece->pdv=32;
            piece->puissance = 24;
            piece->armure=8;
            piece->block=30;
            piece->portee=5;
            piece->deplacement=4;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame;
            piece->frame_interface=frame;
        break;
        case priest:
            piece->classe = priest;
            piece->pdv=34;
            piece->puissance = 12;
            piece->armure=2;
            piece->block=10;
            piece->portee=3;
            piece->deplacement=3;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame;
            piece->frame_interface=frame;
        break;
        case magician:
            piece->classe = magician;
            piece->pdv=30;
            piece->puissance = 28;
            piece->armure=3;
            piece->block=40;
            piece->portee=4;
            piece->deplacement=3;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame;
            piece->frame_interface=frame;
        break;
        case faucheuse:
            piece->classe = faucheuse;
            piece->pdv=25;
            piece->puissance = 35;
            piece->armure=10;
            piece->block=30;
            piece->portee=2;
            piece->deplacement=5;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame;
            piece->frame_interface=frame;
        break;
        case ange:
            piece->classe = ange;
            piece->pdv=40;
            piece->puissance = 22;
            piece->armure=0;
            piece->block=30;
            piece->portee=1;
            piece->deplacement=4;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame2;
            piece->frame_interface=frame2;
        break;
        default: printf("Cette classe n'existe pas.\n");
    }
    return (piece);
}

int piece_existe(piece_t * piece){
    return(!(piece==NULL));
}


int destruction_piece(piece_t * piece){
    if(piece_existe(piece)){
        free(piece);
        (piece)=NULL;
        return 1;
    }
    return 0;
}

void pathfinding(case_t terrain[N][M], int x, int y){
  int deplacement[N][M];
  int i,j;
  int cpt;
    //initialisation des deplacements possibles
    for (i=0;i<N;i++){//parcours du terrain
      for(j=0;j<M;j++){
        terrain[i][j].deplacement=0;
      }
    }


    for (i=0;i<N;i++){//parcours du terrain
        for(j=0;j<M;j++){
            if (terrain[i][j].type==5 || terrain[i][j].piece!=NULL){
                deplacement[i][j]=-1;
            }//case d'eau deplacement impossible
            else{
              deplacement[i][j]=0;
            }
        }


    }
    deplacement[x][y]=1;//case ou se trouve la piece

    int depla;
    //désavantage de deplacement qd le deplacement commence sur une case foret ou montagne.
    if(terrain[x][y].type==6 || terrain[x][y].type==7){
      depla=terrain[x][y].piece->deplacement -1;
    }else{
      depla=terrain[x][y].piece->deplacement;
    }


    for(cpt=1;cpt<=depla;cpt++){//tant qu'il y a des point de deplacement
        for (i=0;i<N;i++){//parcours du terrain
            for(j=0;j<M;j++){
                if (deplacement[i][j]==cpt){
                    if(i+1<N && deplacement[i+1][j]==0)
                        deplacement[i+1][j]=cpt+1;
                    if(i-1>=0 && deplacement[i-1][j]==0)
                        deplacement[i-1][j]=cpt+1;
                    if(j+1<M && deplacement[i][j+1]==0)
                        deplacement[i][j+1]=cpt+1;
                    if(j-1>=0 && deplacement[i][j-1]==0)
                        deplacement[i][j-1]=cpt+1;

                }

            }
        }
    }

    for (i=0;i<N;i++){//parcours du terrain
        for(j=0;j<M;j++){
            if(deplacement[i][j]>1){
              terrain[i][j].deplacement=1;
            }
        }
    }

}




void move(case_t terrain[N][M],int x,int y,int joueur,joueurs_t tab[J]){
  if(tab[joueur].pts_action_actu>0){
    if(terrain[x][y].deplacement==1){
      for(int compteur=0;compteur<N;compteur++){
        for(int compteur2=0;compteur2<M; compteur2++){
          if(terrain[compteur][compteur2].piece && terrain[compteur][compteur2].piece->select==1 && terrain[compteur][compteur2].piece->joueur==joueur){


            terrain[x][y].piece=malloc(sizeof(piece_t));
            terrain[x][y].piece->classe = terrain[compteur][compteur2].piece->classe;
            terrain[x][y].piece->pdv = terrain[compteur][compteur2].piece->pdv;
            terrain[x][y].piece->puissance = terrain[compteur][compteur2].piece->puissance;
            terrain[x][y].piece->armure = terrain[compteur][compteur2].piece->armure;
            terrain[x][y].piece->block = terrain[compteur][compteur2].piece->block;
            terrain[x][y].piece->portee = terrain[compteur][compteur2].piece->portee;
            terrain[x][y].piece->deplacement = terrain[compteur][compteur2].piece->deplacement;
            terrain[x][y].piece->select = terrain[compteur][compteur2].piece->select;
            terrain[x][y].piece->joueur = terrain[compteur][compteur2].piece->joueur;
            terrain[x][y].piece->direction = terrain[compteur][compteur2].piece->direction;
            terrain[x][y].piece->kill = terrain[compteur][compteur2].piece->kill;
            terrain[x][y].piece->frame = terrain[compteur][compteur2].piece->frame;
            terrain[x][y].piece->frame_interface = terrain[compteur][compteur2].piece->frame_interface;


            //destruction_piece(terrain[compteur][compteur2].piece);
            free(terrain[compteur][compteur2].piece);
            terrain[compteur][compteur2].piece=NULL;

            terrain[x][y].piece->select=1;
            //pathfinding(terrain,x,y);

            tab[joueur].pts_action_actu--;

            return;
          }
        }
      }

    }
  }
}
void pathfinding_combat(case_t terrain[N][M], int x, int y,int joueur_actu){
  int attaque[N][M];
  int i,j;
  int cpt;
  //initialisation des attaques possibles
  for (i=0;i<N;i++){//parcours du terrain
      for(j=0;j<M;j++){
        terrain[i][j].attaque=0;
      }
  }

  for (i=0;i<N;i++){//parcours du terrain
      for(j=0;j<M;j++){
        attaque[i][j]=0;
      }
  }

    attaque[x][y]=1;//case ou se trouve la piece
    //gestion du terrain
    int portee_attaquant;
    if(terrain[x][y].type!=7){
      portee_attaquant=terrain[x][y].piece->portee;
    }else{
      portee_attaquant=terrain[x][y].piece->portee+1;
    }

    for(cpt=1;cpt<=portee_attaquant;cpt++){//tant qu'il y a des point de la portée
        for (i=0;i<N;i++){//parcours du terrain
            for(j=0;j<M;j++){
                if (attaque[i][j]==cpt){
                  if(i+1<N && attaque[i+1][j]==0)
                    attaque[i+1][j]=cpt+1;//ICI
                  if(i-1>=0 && attaque[i-1][j]==0)
                    attaque[i-1][j]=cpt+1;
                  if(j+1<N && attaque[i][j+1]==0)
                    attaque[i][j+1]=cpt+1;
                  if(j-1>=0 && attaque[i][j-1]==0)
                    attaque[i][j-1]=cpt+1;
                }

            }
        }
    }
    for (i=0;i<N;i++){
      for(j=0;j<M;j++){
        if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu && attaque[i][j]!=0)
          attaque[i][j]=-2;
      }
    }

    for (i=0;i<N;i++){//parcours du terrain
        for(j=0;j<M;j++){
            if(attaque[i][j]>1 && terrain[i][j].piece && terrain[i][j].piece->joueur!=joueur_actu && terrain[x][y].piece->classe!=priest)
              terrain[i][j].attaque=1;
            if(attaque[i][j]==-2 && terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu && terrain[x][y].piece->classe==priest)
              terrain[i][j].attaque=1;
        }
    }
}

int calc_block(case_t terrain [N][M],int x_att, int y_att, int x_def,int y_def){
    int pos_block[N][M];
    int cpt=1;
    int block=0;
    //init matrice a 0
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
          pos_block[i][j]=-2;
        }
    }
    pos_block[x_def][y_def]=0;
    //diagonales a -1
    while (x_def-cpt>=0 && y_def+cpt<M){
        pos_block[x_def-cpt][y_def+cpt]=-1;
        cpt++;
    }
    cpt=1;
    while (x_def+cpt<N && y_def+cpt<N){
        pos_block[x_def+cpt][y_def+cpt]=-1;
        cpt++;
    }
    cpt=1;
    while (x_def+cpt<N && y_def-cpt>=0){
        pos_block[x_def+cpt][y_def-cpt]=-1;
        cpt++;
    }
    cpt=1;
    while (x_def-cpt>=0 && y_def-cpt>=0){
        pos_block[x_def-cpt][y_def-cpt]=-1;
        cpt++;
    }
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            if ((pos_block[i][j-1]==-1 || pos_block[i][j-1]==0 || pos_block[i][j-1]==1) && j>y_def)
                pos_block[i][j]=1;
        }
    }
    for(int i=0;i<N;i++){
        for(int j=M-1;j>=0;j--){
            if ((pos_block[i][j+1]==-1 || pos_block[i][j+1]==0 || pos_block[i][j+1]==2) && j<y_def)
              pos_block[i][j]=2;
        }
    }
    for(int j=0;j<M;j++){
        for(int i=0;i<N;i++){
            if ((pos_block[i-1][j]==-1 || pos_block[i-1][j]==0 || pos_block[i-1][j]==3) && i>x_def)
                pos_block[i][j]=3;
        }
    }
    for(int j=0;j<M;j++){
        for(int i=0;i<N;i++){
            if (pos_block[i][j]==-2)
                pos_block[i][j]=4;
        }
    }


    if(terrain[x_def][y_def].piece->direction==haut){
        if (pos_block[x_att][y_att]==2){
            block= 0;
        }
        else if (pos_block[x_att][y_att]==3 || pos_block[x_att][y_att]==4 || pos_block[x_att][y_att]==(-1)){
            block=((terrain[x_def][y_def].piece->block)/2);
        }
        else{
            block= (terrain[x_def][y_def].piece->block);
        }
    }
    else if(terrain[x_def][y_def].piece->direction==bas){
        if (pos_block[x_att][y_att]==1){
            block= 0;
        }
        else if (pos_block[x_att][y_att]==3 || pos_block[x_att][y_att]==4 || pos_block[x_att][y_att]==-1){
            block= ((terrain[x_def][y_def].piece->block)/2);
        }
        else{
            block= (terrain[x_def][y_def].piece->block);
        }
    }
    else if(terrain[x_def][y_def].piece->direction==gauche){
        if (pos_block[x_att][y_att]==4){
            block= 0;
        }
        else if (pos_block[x_att][y_att]==1 || pos_block[x_att][y_att]==2 || pos_block[x_att][y_att]==-1){
            block= ((terrain[x_def][y_def].piece->block)/2);
        }
        else{
            block= (terrain[x_def][y_def].piece->block);
        }
    }
    else if(terrain[x_def][y_def].piece->direction==droite){
        if (pos_block[x_att][y_att]==3){
            block= 0;
        }
        else if (pos_block[x_att][y_att]==1 || pos_block[x_att][y_att]==2 || pos_block[x_att][y_att]==-1){
            block= ((terrain[x_def][y_def].piece->block)/2);
        }
        else{
            block= (terrain[x_def][y_def].piece->block);
        }
    }
    return block;
}




void combat(case_t terrain [N][M],int x_att, int y_att, int x_def,int y_def,int joueur,joueurs_t tab[J],degatx_t aff_deg[N*M]){
  if(tab[joueur].pts_action_actu>0){
      if(terrain[x_att][y_att].piece->classe==priest){//soin si la piece selectionné est un pretre
          soin(terrain,x_att,y_att,x_def,y_def,joueur,tab,aff_deg);
          return;
      }
      if(terrain[x_def][y_def].attaque==1){//si la piece attaquée est sur une case a portée
        int blockd=((rand()%(100)) + 1);//att bloquée ou non
        int blockage;
        char variable[20];
        int armure;
        //avantages du terrain de foret
        if(terrain[x_def][y_def].type==6){
          blockage = calc_block(terrain,x_att,y_att,x_def,y_def) + 10;
          armure = terrain[x_def][y_def].piece->armure + 3;
        }else{
          blockage = calc_block(terrain,x_att,y_att,x_def,y_def);
          armure = terrain[x_def][y_def].piece->armure;
        }

        if(blockd>=blockage){

          int deg=((terrain[x_att][y_att].piece->puissance)-armure);

          if(deg>=0){
            terrain[x_def][y_def].piece->pdv-=deg;//armure=absorption de dégats //application des dégats ici

            sprintf(variable, "%d", deg);//sauvegarde des dégat pour affichage
            ajouter_degat_txt(variable,aff_deg,(terrain[x_def][y_def].xImg+50),(terrain[x_def][y_def].yImg),1);


          }else{
            ajouter_degat_txt("0",aff_deg,(terrain[x_def][y_def].xImg+50),(terrain[x_def][y_def].yImg),0);
          }
          if(terrain[x_def][y_def].piece->pdv<=0){   //mort ou non de la piece ennemi
            tab[terrain[x_def][y_def].piece->joueur].nb_unite--;
            terrain[x_att][y_att].piece->kill+=1;
            ajouter_degat_txt("KILL",aff_deg,(terrain[x_def][y_def].xImg),(terrain[x_def][y_def].yImg+20),0);
            free(terrain[x_def][y_def].piece);
            terrain[x_def][y_def].piece=NULL;
            update_stats(terrain,x_att,y_att,joueur,tab);
          }
        }else{
          ajouter_degat_txt("BLOCK",aff_deg,(terrain[x_def][y_def].xImg+50),(terrain[x_def][y_def].yImg),0);
        }
        tab[joueur].pts_action_actu--;
      }
    }
}

void soin(case_t terrain [N][M],int x_att, int y_att, int x_def,int y_def,int joueur,joueurs_t tab[J],degatx_t aff_deg[N*M]){
    char variable[20];
    if(terrain[x_def][y_def].attaque==1){
        terrain[x_def][y_def].piece->pdv+=terrain[x_att][y_att].piece->puissance;
        tab[joueur].pts_action_actu--;
        sprintf(variable, "%d", terrain[x_att][y_att].piece->puissance);
        ajouter_degat_txt(variable,aff_deg,(terrain[x_def][y_def].xImg+50),(terrain[x_def][y_def].yImg),2);
    }
}





void depla_atk_mov(case_t terrain[N][M],int x_bot,int y_bot,int joueur_actu,joueurs_t tab[J]){
  //V2
  //addition de la difference des pos x et y de l'unité par rapport a ses allié,
  //prendre la piece la plus proche et se rapprocher au max des ses coordonné x et y tt en restant dans la range de déplacement
  //int var4=rand()%2; //choix vers ennemi ou allie

  if(terrain[x_bot][y_bot].piece && a_portee(terrain,x_bot,y_bot,joueur_actu)==0 ){
    fprintf(stderr,"VERS ennemi\n");
    depla_ennem_plus_proche(terrain,x_bot,y_bot,joueur_actu,tab);
  }else if(terrain[x_bot][y_bot].piece && reste_allie(terrain,joueur_actu)>1 && (allie_adjacent(terrain,x_bot,y_bot,joueur_actu))==0){
    fprintf(stderr,"VERS allie\n");
    depla_allie_plus_proche(terrain,x_bot,y_bot,joueur_actu,tab);
  }else{
    fprintf(stderr,"ALEA DEPLA\n");
    move_alea(terrain,x_bot,y_bot,joueur_actu,tab);
  }
}

void centrer_camera(case_t terrain[N][M],int x,int y,int largeur,int hauteur){
	int x_centre,y_centre,diff_x,diff_y;
	x_centre=largeur/2;
	y_centre=hauteur/2;
	diff_x=abs(x_centre-x);
	diff_y=abs(y_centre-y);

	if(x<=x_centre){
		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				terrain[i][j].xImg+=diff_x;
				terrain[i][j].x1+=diff_x;
				terrain[i][j].x2+=diff_x;
				terrain[i][j].x3+=diff_x;
				terrain[i][j].x4+=diff_x;
			}
		}
	}else{
		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				terrain[i][j].xImg-=diff_x;
				terrain[i][j].x1-=diff_x;
				terrain[i][j].x2-=diff_x;
				terrain[i][j].x3-=diff_x;
				terrain[i][j].x4-=diff_x;
			}
		}
	}

	if(y<=y_centre){
		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				terrain[i][j].yImg+=diff_y;
				terrain[i][j].y1+=diff_y;
				terrain[i][j].y2+=diff_y;
				terrain[i][j].y3+=diff_y;
				terrain[i][j].y4+=diff_y;
			}
		}
	}else{
		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				terrain[i][j].yImg-=diff_y;
				terrain[i][j].y1-=diff_y;
				terrain[i][j].y2-=diff_y;
				terrain[i][j].y3-=diff_y;
				terrain[i][j].y4-=diff_y;
			}
		}
	}
}



void ajouter_degat_txt(char txt[20],degatx_t aff_deg[AFF_DEG],int x,int y,int c){
	for(int i=0;i<AFF_DEG;i++){
		if(aff_deg[i].time<=0){
      strcpy(aff_deg[i].txt,txt);
      aff_deg[i].pos_x=x;
      aff_deg[i].pos_y=y;
      aff_deg[i].c=c;
      aff_deg[i].time=100;
      return;
    }
	}
}



void clean_degat_txt(degatx_t aff_deg[AFF_DEG]){
  for(int i=0;i<AFF_DEG;i++){
		if(aff_deg[i].time<=0 && !vide(aff_deg)){
      for(int j=i;j<(AFF_DEG)-1;j++){
        aff_deg[j].time=aff_deg[j+1].time;
        aff_deg[j].pos_x=aff_deg[j+1].pos_x;
        aff_deg[j].pos_y=aff_deg[j+1].pos_y;
        aff_deg[j].c=aff_deg[j+1].c;
        strcpy(aff_deg[j].txt,aff_deg[j+1].txt);
      }
    }else{
      if(strcmp(aff_deg[i].txt,"MORT")){
        aff_deg[i].time-=10;
        aff_deg[i].pos_x+=(aff_deg[i].time/10);
        aff_deg[i].pos_y-=(aff_deg[i].time/10);
      }else{
        aff_deg[i].time-=10;
        aff_deg[i].pos_y-=(aff_deg[i].time/15);
      }
    }
  }
}

int vide(degatx_t aff_deg[AFF_DEG]){
  for(int i=0;i<AFF_DEG;i++){
    if(aff_deg[i].time>0)
      return 0;
  }
  return 1;
}



int a_portee(case_t terrain[N][M],int x_bot,int y_bot,int joueur_actu){
  pathfinding_combat(terrain,x_bot,y_bot,joueur_actu);
  int nb=0;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].attaque==1){
        nb++;
      }
    }
  }
  return nb;
}



void attaquer_meilleur_cible(case_t terrain[N][M],int x_bot,int y_bot,int nb_ennemies_portee,int joueur_actu,joueurs_t tab[J],degatx_t aff_deg[AFF_DEG]){
  pathfinding_combat(terrain,x_bot,y_bot,joueur_actu);
  if(terrain[x_bot][y_bot].piece->classe==priest){//on heal l'allié avec le moin de pv si on est soigneur ou celui qui a le plus d'ennemi adjacent si tt les points de vie sont supérieur 25
    int x_low,y_low,pv_low=-1;
    int tab_ennemi_ont_portee[N][M];
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        tab_ennemi_ont_portee[i][j]=0;
      }
    }
    for(int i=0;i<N;i++){ //minimum de pv des alliés
      for(int j=0;j<M;j++){
        if(terrain[i][j].attaque==1 && pv_low==-1){
          x_low=i;
          y_low=j;
          pv_low=terrain[i][j].piece->pdv;
        }else if(terrain[i][j].attaque==1 && pv_low>terrain[i][j].piece->pdv){
          x_low=i;
          y_low=j;
          pv_low=terrain[i][j].piece->pdv;
        }
      }
    }
    if(pv_low<=25){//soin si pv <= a 25

      combat(terrain,x_bot,y_bot,x_low,y_low,joueur_actu,tab,aff_deg);

    }else{ //sinon on heal l'allié avec le plus d'ennemi adjacent

      for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
          if(terrain[i][j].piece && terrain[i][j].piece->joueur!=joueur_actu){
            pathfinding_combat(terrain,i,j,terrain[x_bot][y_bot].piece->joueur);
            for(int compteur=0;compteur<N;compteur++){  //a optimiser pour grille plus grande
              for(int compteur2=0;compteur2<M;compteur2++){
                if(terrain[compteur][compteur2].attaque==1){
                  tab_ennemi_ont_portee[compteur][compteur2]+=1;
                }
              }
            }
          }
        }
      }
      int max_atk=0,x=x_bot,y=y_bot;
      pathfinding_combat(terrain,x_bot,y_bot,joueur_actu);
      for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
          if(terrain[i][j].attaque==1 && tab_ennemi_ont_portee[i][j]>max_atk){
            max_atk=tab_ennemi_ont_portee[i][j];
            x=i;
            y=j;
          }
        }
      }
      combat(terrain,x_bot,y_bot,x,y,joueur_actu,tab,aff_deg);
    }
  }else{ //cas ou ce n'est pas un soigneur
    int x_low,y_low,pv_low=-1;
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        if(terrain[i][j].attaque==1 && pv_low==-1){
          x_low=i;
          y_low=j;
          pv_low=terrain[i][j].piece->pdv;
        }else if(terrain[i][j].attaque==1 && pv_low>terrain[i][j].piece->pdv){
          x_low=i;
          y_low=j;
          pv_low=terrain[i][j].piece->pdv;
        }
      }
    }
    combat(terrain,x_bot,y_bot,x_low,y_low,joueur_actu,tab,aff_deg); //attaque l'ennemi avec le moin de pv
  }

}



int reste_ennemi(case_t terrain[N][M],int joueur_actu){
  int nb=0;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur != joueur_actu){
        nb++;
      }
    }
  }
  return nb;
}

int reste_allie(case_t terrain[N][M],int joueur_actu){
  int nb=0;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur == joueur_actu){
        nb++;
      }
    }
  }
  return nb;
}




void depla_allie_plus_proche(case_t terrain[N][M],int x_bot,int y_bot,int joueur_actu,joueurs_t tab[J]){
  //calcule de l'allie le plus proche et sauvegarde de ses position x et y
  int x_allie,y_allie;
  int distance_allie=-1;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu && distance_allie==-1 && (x_bot!=i || y_bot!=j)){
        x_allie=i;
        y_allie=j;
        distance_allie=(abs(x_bot-x_allie)+abs(y_bot-y_allie));
      }else if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu && distance_allie>(abs(x_bot-i)+abs(y_bot-j)) && (x_bot!=i || y_bot!=j)){
        x_allie=i;
        y_allie=j;
        distance_allie=(abs(x_bot-x_allie)+abs(y_bot-y_allie));
      }
    }
  }

  //deplacement au plus pret
  move_longue_range(terrain,x_bot,y_bot,x_allie,y_allie,joueur_actu,tab);
}




void depla_ennem_plus_proche(case_t terrain[N][M],int x_bot,int y_bot,int joueur_actu,joueurs_t tab[J]){
  //calcule de l'ennemi le plus proche et sauvegarde de ses position x et y
  int x_allie,y_allie;
  int distance_allie=-1;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur!=joueur_actu && distance_allie==-1){
        x_allie=i;
        y_allie=j;
        distance_allie=(abs(x_bot-x_allie)+abs(y_bot-y_allie));
      }else if(terrain[i][j].piece && terrain[i][j].piece->joueur!=joueur_actu && distance_allie>(abs(x_bot-i)+abs(y_bot-j))){
        x_allie=i;
        y_allie=j;
        distance_allie=(abs(x_bot-x_allie)+abs(y_bot-y_allie));
      }
    }
  }

  //deplacement au plus pret
  move_longue_range(terrain,x_bot,y_bot,x_allie,y_allie,joueur_actu,tab);

}



int allie_adjacent(case_t terrain[N][M],int x_bot,int y_bot,int joueur_actu){
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(i+1<N && terrain[i+1][j].piece && terrain[i+1][j].piece->joueur==joueur_actu){
        return 1;
      }
      if(j+1<N && terrain[i][j+1].piece && terrain[i][j+1].piece->joueur==joueur_actu){
        return 1;
      }
      if(i-1>=0 && terrain[i-1][j].piece && terrain[i-1][j].piece->joueur==joueur_actu){
        return 1;
      }
      if(j-1>=0 && terrain[i][j-1].piece && terrain[i][j-1].piece->joueur==joueur_actu){
        return 1;
      }
    }
  }
  return 0;

}


void move_longue_range(case_t terrain[N][M], int x, int y,int x_dest,int y_dest,int joueur_actu,joueurs_t tab[J]){
  int deplacement[N][M];
  int i,j;
  int cpt;
    //initialisation des deplacements possibles

  for (i=0;i<N;i++){//parcours du terrain
      for(j=0;j<M;j++){
          if ((terrain[i][j].type==5) && terrain[i][j].piece!=NULL){
              deplacement[i][j]=-1;
          }//case d'eau deplacement impossible
          else{
            deplacement[i][j]=0;
          }
      }


  }
  deplacement[x][y]=1;//case ou se trouve la piece
  deplacement[x_dest][y_dest]=0;//case ou se trouve la piece a rejoindre

  for(cpt=1;deplacement[x_dest][y_dest]==0;cpt++){//tant qu'il y a des point de deplacement
      for (i=0;i<N;i++){//parcours du terrain
          for(j=0;j<M;j++){
              if (deplacement[i][j]==cpt){
                  if(i+1<N && deplacement[i+1][j]==0)
                      deplacement[i+1][j]=cpt+1;
                  if(i-1>=0 && deplacement[i-1][j]==0)
                      deplacement[i-1][j]=cpt+1;
                  if(j+1<M && deplacement[i][j+1]==0)
                      deplacement[i][j+1]=cpt+1;
                  if(j-1>=0 && deplacement[i][j-1]==0)
                      deplacement[i][j-1]=cpt+1;

              }

          }
      }
  }

  int a=x_dest;
  int b=y_dest;
  int var=deplacement[x_dest][y_dest];
  pathfinding(terrain,x,y);
  while(terrain[a][b].deplacement!=1){
    fprintf(stderr,"boucle %d\n",var);
    if(a+1<N && deplacement[a+1][b]==var-1){
      a++;
      var--;
    }
    else if(a-1>=0 && deplacement[a-1][b]==var-1){
      a--;
      var--;
    }
    else if(b+1<M && deplacement[a][b+1]==var-1){
      b++;
      var--;
    }
    else if(b-1>=0 && deplacement[a][b-1]==var-1){
      b--;
      var--;
    }else{  //bug bizard, pas censer rentrer dans le else ici bas
      fprintf(stderr,"!!! ALEA !!!\n");
      move_alea(terrain,a,b,joueur_actu,tab);
      return;
    }
  }




  move(terrain,a,b,joueur_actu,tab);

}

void move_alea(case_t terrain[N][M],int x,int y, int joueur,joueurs_t tab[J]){
  pathfinding(terrain,x,y);
  int ini=0,x_dest,y_dest;
  for (int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].deplacement==1 && ini==0){
        x_dest=i;
        y_dest=j;
        ini=1;
      }else if(terrain[i][j].deplacement==1 && rand()%((N+M)/2)){
        x_dest=i;
        y_dest=j;
      }
    }
  }
  move(terrain,x_dest,y_dest,joueur,tab);
}



void update_stats(case_t terrain[N][M],int x,int y,int joueur_actu,joueurs_t tab[J]){



        //++++++KNIGHT++++++++//
  if(terrain[x][y].piece->classe==knight){
    //===bonus a chaque kill=====//
    terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
    terrain[x][y].piece->armure+=1;
    //===bonus tous les 3 kills=====//
    if(terrain[x][y].piece->kill % 3==0){
      terrain[x][y].piece->deplacement+=1;
      terrain[x][y].piece->pdv+=10;
    }
    //===bonus de vétérant de la guerre(10 kills)===//
    if(terrain[x][y].piece->kill==10){
      terrain[x][y].piece->deplacement+=1;
      terrain[x][y].piece->portee+=1;
      terrain[x][y].piece->puissance+=10;
      terrain[x][y].piece->armure+=5;
      terrain[x][y].piece->block+=10;
    }



        //++++++SCOUT++++++++//
  }else if(terrain[x][y].piece->classe==scout){
    //===bonus a chaque kill=====//
    terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
    //===bonus tous les 2 kills=====//
    if(terrain[x][y].piece->kill % 2==0){
      terrain[x][y].piece->deplacement+=1;
    }
    //===bonus tous les 3 kills=====//
    if(terrain[x][y].piece->kill % 3==0){
      terrain[x][y].piece->portee+=1;
      terrain[x][y].piece->pdv+=5;
    }
    //===bonus de vétérant de la guerre(10 kills)===//
    if(terrain[x][y].piece->kill==10){
      terrain[x][y].piece->puissance+=13;
      terrain[x][y].piece->armure+=3;
      terrain[x][y].piece->block+=20;
    }





        //++++++MAGICIAN++++++++//
  }else if(terrain[x][y].piece->classe==magician){
    //===bonus a chaque kill=====//
    terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
    terrain[x][y].piece->pdv+=terrain[x][y].piece->kill;
    //===bonus tous les 5 kills=====//
    if(terrain[x][y].piece->kill % 5==0){
      terrain[x][y].piece->portee+=2;
      terrain[x][y].piece->pdv+=15;
      terrain[x][y].piece->deplacement+=1;
    }
    //===bonus de vétérant de la guerre(10 kills)===//
    if(terrain[x][y].piece->kill==10){
      terrain[x][y].piece->puissance+=14;
      terrain[x][y].piece->armure+=3;
      terrain[x][y].piece->block+=5;
    }





       //++++++FAUCHEUSE++++++++//
  }else if(terrain[x][y].piece->classe==faucheuse){
      //===bonus a chaque kill=====//
    terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
    //===bonus tous les 5 kills=====//
    if(terrain[x][y].piece->kill % 5==0){
      terrain[x][y].piece->deplacement+=1;
      if(terrain[x][y].piece->block<=90){
        terrain[x][y].piece->block+=5;
      }
    }
    //===bonus de vétérant de la guerre(10 kills)===//
    if(terrain[x][y].piece->kill==10){
      terrain[x][y].piece->puissance+=15;
      terrain[x][y].piece->portee+=1;
      terrain[x][y].piece->block+=10;
    }




     //++++++ANGE++++++++//
  }else if(terrain[x][y].piece->classe==ange){
      //===bonus a chaque kill=====//
    terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
    tab[joueur_actu].pts_action_actu+=1;
    terrain[x][y].piece->pdv+=(terrain[x][y].piece->kill)*3;
    //===bonus tous les 5 kills=====//
    if(terrain[x][y].piece->kill % 5==0){
      terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
      terrain[x][y].piece->deplacement+=1;
    }
    //===bonus de vétérant de la guerre(10 kills)===//
    if(terrain[x][y].piece->kill==10){
      terrain[x][y].piece->puissance+=15;
      terrain[x][y].piece->armure+=5;
      terrain[x][y].piece->deplacement+=1;
    }



      //++++++AUTRE++++++++//
  }else if(terrain[x][y].piece->classe==priest){ //autre unité

  }

}




void IA_blockage_direction(case_t terrain[N][M],int x_def,int y_def,int joueur_actu){
  int pos_block[N][M];
  int cpt=1;
  //init matrice a 0
  for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        pos_block[i][j]=-2;
      }
  }
  pos_block[x_def][y_def]=0;
  //diagonales a -1
  while (x_def-cpt>=0 && y_def+cpt<M){
      pos_block[x_def-cpt][y_def+cpt]=-1;
      cpt++;
  }
  cpt=1;
  while (x_def+cpt<N && y_def+cpt<N){
      pos_block[x_def+cpt][y_def+cpt]=-1;
      cpt++;
  }
  cpt=1;
  while (x_def+cpt<N && y_def-cpt>=0){
      pos_block[x_def+cpt][y_def-cpt]=-1;
      cpt++;
  }
  cpt=1;
  while (x_def-cpt>=0 && y_def-cpt>=0){
      pos_block[x_def-cpt][y_def-cpt]=-1;
      cpt++;
  }
  for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
          if ((pos_block[i][j-1]==-1 || pos_block[i][j-1]==0 || pos_block[i][j-1]==1) && j>y_def)
              pos_block[i][j]=1;
      }
  }
  for(int i=0;i<N;i++){
      for(int j=M-1;j>=0;j--){
          if ((pos_block[i][j+1]==-1 || pos_block[i][j+1]==0 || pos_block[i][j+1]==2) && j<y_def)
            pos_block[i][j]=2;
      }
  }
  for(int j=0;j<M;j++){
      for(int i=0;i<N;i++){
          if ((pos_block[i-1][j]==-1 || pos_block[i-1][j]==0 || pos_block[i-1][j]==3) && i>x_def)
              pos_block[i][j]=3;
      }
  }
  for(int j=0;j<M;j++){
      for(int i=0;i<N;i++){
          if (pos_block[i][j]==-2)
              pos_block[i][j]=4;
      }
  }
  //on compte le nb d'unité dans chaque direction, on ignore les diagonnales car une unité sur n'ipmporte quel diagonnale fera toujours 50% du block
  int nb_nord=0,nb_sud=0,nb_ouest=0,nb_est=0;

  for (int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur != joueur_actu){
        if(pos_block[i][j]==1){
          nb_nord++;
        }else if(pos_block[i][j]==2){
          nb_sud++;
        }else if(pos_block[i][j]==3){
          nb_ouest++;
        }else if(pos_block[i][j]==4){
          nb_est++;
        }
      }
    }
  }

  //on se tourne vers la meilleur direction
  if(nb_nord>=nb_sud && nb_nord>=nb_ouest && nb_nord>=nb_est){
    terrain[x_def][y_def].piece->direction=haut;

  }else if(nb_sud>=nb_nord && nb_sud>=nb_ouest && nb_sud>=nb_est){
    terrain[x_def][y_def].piece->direction=bas;

  }else if(nb_ouest>=nb_sud && nb_ouest>=nb_nord && nb_ouest>=nb_est){
    terrain[x_def][y_def].piece->direction=gauche;

  }else{
    terrain[x_def][y_def].piece->direction=droite;
  }

}



SDL_Rect afficher_anim(int compteur_anim,classe_t classe,case_t terrain[N][M],int compteur,int compteur2,int inter){
  SDL_Rect img_anim;
  img_anim.h=100;
  img_anim.w=100;
    //anim a 2 images
  if(inter==0){
    if(classe==knight || classe==scout || classe==magician || classe==priest){
      if(compteur_anim%VITESSE_ANIM==0){
        if(terrain[compteur][compteur2].piece->frame==2){
          terrain[compteur][compteur2].piece->frame=1;
        }else{
          terrain[compteur][compteur2].piece->frame++;
        }
      }
      if(terrain[compteur][compteur2].piece->frame==1){
          img_anim.x=0;
          img_anim.y=0;
      }else if(terrain[compteur][compteur2].piece->frame==2){
          img_anim.x=100;
          img_anim.y=0;
      }
    }
    //ANIM DE L'ANGE
    else if(classe==ange){
      if(compteur_anim%VITESSE_ANIM==0){
        if(terrain[compteur][compteur2].piece->frame==10){
          terrain[compteur][compteur2].piece->frame=1;
        }else{
          terrain[compteur][compteur2].piece->frame++;
        }
      }
      if(terrain[compteur][compteur2].piece->frame==1 || terrain[compteur][compteur2].piece->frame>9){
          img_anim.x=0;
          img_anim.y=0;
      }else if(terrain[compteur][compteur2].piece->frame%2==0){
          img_anim.x=100;
          img_anim.y=0;
      }else{
          img_anim.x=0;
          img_anim.y=100;
      }
    }
  }else{ //anim de l'interface en haut a gauche
    if(classe==knight || classe==scout || classe==magician || classe==priest){
      if(compteur_anim%VITESSE_ANIM==0){
        if(terrain[compteur][compteur2].piece->frame_interface==2){
          terrain[compteur][compteur2].piece->frame_interface=1;
        }else{
          terrain[compteur][compteur2].piece->frame_interface++;
        }
      }
      if(terrain[compteur][compteur2].piece->frame_interface==1){
          img_anim.x=0;
          img_anim.y=0;
      }else if(terrain[compteur][compteur2].piece->frame_interface==2){
          img_anim.x=100;
          img_anim.y=0;
      }
    }
    else if(classe==ange){
      if(compteur_anim%VITESSE_ANIM==0){
        if(terrain[compteur][compteur2].piece->frame_interface==10){
          terrain[compteur][compteur2].piece->frame_interface=1;
        }else{
          terrain[compteur][compteur2].piece->frame_interface++;
        }
      }
      if(terrain[compteur][compteur2].piece->frame_interface==1 || terrain[compteur][compteur2].piece->frame_interface>9){
          img_anim.x=0;
          img_anim.y=0;
      }else if(terrain[compteur][compteur2].piece->frame_interface%2==0){
          img_anim.x=100;
          img_anim.y=0;
      }else{
          img_anim.x=0;
          img_anim.y=100;
      }
    }

  }



  return img_anim;
}


void carte_valide(case_t terrain[N][M]){
   int cases[N][M];
   for(int i=0;i<N;i++){
     for(int j=0;j<M;j++){
       if(terrain[i][j].type==5){
         cases[i][j]=-1;
       }else{
         cases[i][j]=0;
       }
     }
   }
   //coin =1
   cases[0][0]=1;
   cases[N-1][M-1]=1;
   cases[0][M-1]=1;
   cases[N-1][0]=1;

   for(int a=0;a<N*M;a++){
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        if (cases[i][j]==1){
          if(i+1<N && cases[i+1][j]==0)
              cases[i+1][j]=1;
          if(i-1>=0 && cases[i-1][j]==0)
             cases[i-1][j]=1;
          if(j+1<M && cases[i][j+1]==0)
             cases[i][j+1]=1;
          if(j-1>=0 && cases[i][j-1]==0)
             cases[i][j-1]=1;
         }
       }
     }
   }
   //verification de la map
   for(int i=0;i<N;i++){
     for(int j=0;j<M;j++){
       if(cases[i][j]==0){
         cases[i][j]=2;
       }
     }
   }
   //correction de la map
   for(int i=0;i<N;i++){
     for(int j=0;j<M;j++){
       if(cases[i][j]==2){
         terrain[i][j].type=5;
       }
     }
   }
   return;
 }



/////////////////////////////RESEAU////////////////////////////////////////


void fin(int sig){
	printf("fin du serveur");
}

int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ( (he = gethostbyname( hostname ) ) == NULL)
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++)
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }

    return 1;
}


void view_ip()
{
  char s[256];
  struct hostent *host;
  struct in_addr **adr;
  if (!gethostname(s, 256))
    if ((host = gethostbyname(s)) != NULL)
      for (adr = (struct in_addr **)host->h_addr_list; *adr; adr++)
          printf("IP : %s\n", inet_ntoa(**adr));
}
void send_game_state(int client_socket,case_t terrain[N][M]){
	int i,j;
	for (i=0;i<N;i++){
		for(j=0;j<M;j++){
            /*//case
			send(client_socket,terrain[i][j].type,sizeof(int),0);
            send(client_socket,terrain[i][j].climat,sizeof(i),0);
            send(client_socket,terrain[i][j].xImg,sizeof(int),0);
            send(client_socket,terrain[i][j].yImg,sizeof(i),0);
            send(client_socket,terrain[i][j].x1,sizeof(int),0);
            send(client_socket,terrain[i][j].x2,sizeof(i),0);
            send(client_socket,terrain[i][j].x3,sizeof(int),0);
            send(client_socket,terrain[i][j].x4,sizeof(i),0);
            send(client_socket,terrain[i][j].y1,sizeof(int),0);
            send(client_socket,terrain[i][j].y2,sizeof(i),0);
            send(client_socket,terrain[i][j].y3,sizeof(int),0);
            send(client_socket,terrain[i][j].y4,sizeof(i),0);

            //piece
            if(piece_existe(terrain[i][j])){
                send(client_socket,terrain[i][j].piece->classe,sizeof(int),0);
                send(client_socket,terrain[i][j].piece->pdv,sizeof(int),0);
                send(client_socket,terrain[i][j].piece->puissance,sizeof(i),0);
                send(client_socket,terrain[i][j].piece->armure,sizeof(int),0);
                send(client_socket,terrain[i][j].piece->block,sizeof(int),0);
                send(client_socket,terrain[i][j].piece->portee,sizeof(int),0);
                send(client_socket,terrain[i][j].piece->deplacement,sizeof(int),0);
                send(client_socket,terrain[i][j].piece->select,sizeof(int),0);
                send(client_socket,terrain[i][j].piece->joueur,sizeof(int),0);
                send(client_socket,terrain[i][j].piece->direction,sizeof(int),0);
                send(client_socket,terrain[i][j].piece->kill,sizeof(int),0);*/
            init_piece(buffer[i][j].piece,terrain[i][j].piece->joueur);
            buffer[i][j]=terrain[i][j];
            send(client_socket,buffer[i][j],sizeof(buffer),0);
            }
		}
	}
	/*printf("quel est votre message : ");
	scanf(" %[^\n]s", buffer);
	sprintf(msg, "MSG %s", buffer);
	send(client_socket, msg, strlen(msg), 0); //on augmente la taille de 4 pour l'entête
	// lecture de la réponse
	memset(buffer, 0, sizeof(buffer));*/
	recv(client_socket,buffer,512,0);
	printf("[client] reponse du client: '%s'\n", buffer);
}

int setup_serv (int * client_socket)
{
	int ma_socket;
	struct sockaddr_in mon_address, client_address;
	unsigned int mon_address_longueur, lg;
	bzero(&mon_address,sizeof(mon_address));
	mon_address.sin_port = htons(30000);
	mon_address.sin_family = AF_INET;
	mon_address.sin_addr.s_addr = htonl(INADDR_ANY);

	char *hostname = "localhost";
    char ip[100];

    hostname_to_ip(hostname , ip);
	fprintf(stderr, "%s resolved to %s" , hostname , ip);
	view_ip();

	/* creation de socket */
	if ((ma_socket = socket(AF_INET,SOCK_STREAM,0))== -1) {
		printf("Impossible de créer la socket\n");
		exit(0);
	}

	/* bind serveur - socket */
	bind(ma_socket,(struct sockaddr *)&mon_address,sizeof(mon_address));
	/* ecoute sur la socket */
	listen(ma_socket,5);
	/* accept la connexion */
	mon_address_longueur = sizeof(client_address);

    /* on attend que le client se connecte */
	client_socket = accept(ma_socket,
                         (struct sockaddr *)&client_address,
                         &mon_address_longueur);

	int quitter=0;
    while(!quitter)
	{
		memset(buffer, 0, sizeof(buffer));
		lg = recv(client_socket, buffer, 512,0);

		if(strncmp("MSG", buffer, 3)==0){
			printf("[serveur] message reçu : '%s'\n",buffer+4);
			printf("[serveur] envoi de la réponse ");
			sprintf(buffer,"REPONSE DU SERVEUR");
			send(client_socket, buffer, 512, 0);
		} else if(strncmp("QUITTER", buffer, 7) == 0) {
			printf("[serveur] déconnexion demandée : '%s'\n",buffer);
			shutdown(client_socket,2);
			close(client_socket);
			quitter=1;
		} else if(strncmp("BONJOUR", buffer, 7) == 0){
			printf("[serveur] BONJOUR d'un client\n");
			send(client_socket, "BONJOUR", 7, 0);
		} else {
			printf("[serveur] message inconnu : '%s'\n", buffer);
		}
	}
	shutdown(ma_socket,2);
	close(ma_socket);
	return 0;
}

////////////////////CLIENT///////////////////////////////////
void quitter(int to_server_socket){
	printf("[client] envoi message QUITTER au serveur\n");
	send(to_server_socket,QUITTER,7,0);
}

void receive_game_state(int client_socket,case_t terrain[N][M]){

}

int connection_serv (int * to_server_socket)
{
	struct sockaddr_in serveur_addr;
	struct hostent *serveur_info;
	long hostAddr;
	char buffer[512];

	bzero(&serveur_addr,sizeof(serveur_addr));
	hostAddr = inet_addr(SERVEURNAME);
	if ( (long)hostAddr != (long)-1 ){
		bcopy(&hostAddr,&serveur_addr.sin_addr,sizeof(hostAddr));
	} else {
		serveur_info = gethostbyname(SERVEURNAME);
	  	if (serveur_info == NULL) {
			printf("Impossible de récupérer les infos du serveur\n");
			exit(0);
	  	}
	  	bcopy(serveur_info->h_addr,&serveur_addr.sin_addr,serveur_info->h_length);
	}
	serveur_addr.sin_port = htons(30000);
	serveur_addr.sin_family = AF_INET;
	/* creation de la socket */
	if ( (to_server_socket = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		printf("Impossible de créer la socket client\n");
	  	exit(0);
	}
	/* requete de connexion */
	if(connect( to_server_socket, (struct sockaddr *)&serveur_addr, sizeof(serveur_addr)) < 0 ) {
		printf("Impossible de se connecter au serveur\n");
	  	exit(0);
	}
	/* envoie de données et reception */
	send(to_server_socket,"BONJOUR",7,0);
	memset(buffer, 0, sizeof(buffer));
	recv(to_server_socket,buffer,512, 0);
	printf("[client] %s [du serveur]\n", buffer);

	/* fermeture de la connexion */
	shutdown(to_server_socket,2);
	close(to_server_socket);
	return 0;
}
