/*
 * Toxic -- Tox Curses Client
 */

#include <curses.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "../../core/Messenger.h"
#include "../../core/network.h"

#include "windows.h"

#define MAX_FRIENDS_NUM 100

typedef struct {
  uint8_t name[MAX_NAME_LENGTH];
  uint8_t status[MAX_USERSTATUS_LENGTH];
  int     num;

} friend_t;

static friend_t friends[MAX_FRIENDS_NUM];
static int num_friends = 0;


void fix_name(uint8_t* name) {

  // Remove all non alphanumeric characters.
  uint8_t* p = name;
  uint8_t* q = name;

  while(*p != 0) {
    if(isalnum(*p)) {
      *q++ = *p;
    }

    p++;
  }

  *q = 0;
}

int friendlist_nickchange(int num, uint8_t* str, uint16_t len) {

  if(len >= MAX_NAME_LENGTH || num >= num_friends)
    return -1;

  memcpy((char*) &friends[num].name, (char*) str, len);
  friends[num].name[len] = 0;
  fix_name(friends[num].name);

  return 0;
}

int friendlist_statuschange(int num, uint8_t* str, uint16_t len) {

  if(len >= MAX_USERSTATUS_LENGTH || num >= num_friends)
    return -1;

  memcpy((char*) &friends[num].status, (char*) str, len);
  friends[num].status[len] = 0;
  fix_name(friends[num].status);

  return 0;
}

int friendlist_addfriend(int num) {

  if(num_friends == MAX_FRIENDS_NUM)
    return -1;

  friends[num_friends].num = num;
  getname(num, friends[num_friends].name);
  strcpy((char*) friends[num_friends].name, "unknown");
  strcpy((char*) friends[num_friends].status, "unknown");

  num_friends++;
  return 0;
}

static void friendlist_onKey(ToxWindow* self, int key) {

}

static void friendlist_onDraw(ToxWindow* self) {
  size_t i;

  wclear(self->window);

  if(num_friends == 0) {
    wprintw(self->window, "Empty. Add some friends! :-)\n");
  }

  wprintw(self->window, "\n");

  for(i=0; i<num_friends; i++) {
    wprintw(self->window, "[%d] ", friends[i].num);

    attron(A_BOLD);
    wprintw(self->window, "%s ", friends[i].name);
    attroff(A_BOLD);

    wprintw(self->window, "(%s)\n", friends[i].status);
  }

  wrefresh(self->window);
}

static void friendlist_onInit(ToxWindow* self) {

}


ToxWindow new_friendlist() {
  ToxWindow ret;

  ret.onKey = &friendlist_onKey;
  ret.onDraw = &friendlist_onDraw;
  ret.onInit = &friendlist_onInit;
  strcpy(ret.title, "[friends]");

  return ret;
}
