/*
 * Copyright (c) 2015-2017, Davide Galassi. All rights reserved.
 *
 * This file is part of the BeeOS software.
 *
 * BeeOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BeeOS; if not, see <http://www.gnu/licenses/>.
 */

#include "proc.h"
#include "proc/task.h"

pid_t sys_fork(void)
{
    struct task *child, *sib;
    
    child = task_create();
    if (child == NULL)
        return -1;

    if (current_task->pid == child->pid)
        return 0;

    /* Add to the global tasks list */
    list_insert_before(&current_task->tasks, &child->tasks);

    sib = list_container(current_task->children.next, struct task, children);
    if (list_empty(&current_task->children) || sib->pptr != current_task)
        list_insert_after(&current_task->children, &child->children);
    else
        list_insert_before(&sib->sibling, &child->sibling);

    return child->pid;
}