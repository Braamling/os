int pcb_insert_before(pcb *item, pcb *next_item){

	/* Check whether there is a previous node. */
	if(next_item->prev->next != NULL){
		next_item->prev->next = item;
	}else{
		next_item->prev->next = NULL;
	}
	item->prev = next_item->prev;
	next_item->prev = item;
	item->next = next_item;
}

int pcb_insert_after(pcb *item, pcb *prev_item){

	/* Check whether there is a next node. */
	if(prev_item->next->prev != NULL){
		prev_item->next->prev = item;
	}else{
		prev_item->next->prev = NULL;
	}
	item->next = prev_item->next;
	item->prev = prev_item;
	prev_item->next = item;
}

int pcb_move_after(pcb *item, pcb *new_next){
	if(pcb_remove() == -1){
		return -1;
	}
	if(pcb_insert_after(item, new_next) == -1){
		return -1;
	}
	return 0;
}
