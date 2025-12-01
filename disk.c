struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX],
                                       int *queue_cnt,
                                       struct RCB current_request,
                                       struct RCB new_request,
                                       int timestamp) {
    (void)timestamp; 

    if (current_request.request_id == 0 &&
        current_request.arrival_timestamp == 0 &&
        current_request.cylinder == 0 &&
        current_request.address == 0 &&
        current_request.process_id == 0) {
        return new_request;
    }

    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
}

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],
                                          int *queue_cnt) {
    struct RCB nullrcb = {0, 0, 0, 0, 0};

    if (*queue_cnt == 0) {
        return nullrcb;
    }

    int idx = 0;
    for (int i = 1; i < *queue_cnt; i++) {
        if (request_queue[i].arrival_timestamp <
            request_queue[idx].arrival_timestamp) {
            idx = i;
        }
    }

    struct RCB next = request_queue[idx];

    for (int i = idx; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_cnt)--;

    return next;
}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],
                                       int *queue_cnt,
                                       struct RCB current_request,
                                       struct RCB new_request,
                                       int timestamp) {
    (void)timestamp;

    if (current_request.request_id == 0 &&
        current_request.arrival_timestamp == 0 &&
        current_request.cylinder == 0 &&
        current_request.address == 0 &&
        current_request.process_id == 0) {
        return new_request;
    }

    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],
                                          int *queue_cnt,
                                          int current_cylinder) {
    struct RCB nullrcb = {0, 0, 0, 0, 0};

    if (*queue_cnt == 0) {
        return nullrcb;
    }

    int idx = -1;
    int best_dist = 0;

    for (int i = 0; i < *queue_cnt; i++) {
        int cyl = request_queue[i].cylinder;
        int dist = (cyl >= current_cylinder) ?
                   (cyl - current_cylinder) :
                   (current_cylinder - cyl);

        if (idx == -1 ||
            dist < best_dist ||
           (dist == best_dist &&
            request_queue[i].arrival_timestamp <
            request_queue[idx].arrival_timestamp)) {
            idx = i;
            best_dist = dist;
        }
    }

    struct RCB next = request_queue[idx];

    for (int i = idx; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_cnt)--;

    return next;
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],
                                       int *queue_cnt,
                                       struct RCB current_request,
                                       struct RCB new_request,
                                       int timestamp) {
    (void)timestamp;

    if (current_request.request_id == 0 &&
        current_request.arrival_timestamp == 0 &&
        current_request.cylinder == 0 &&
        current_request.address == 0 &&
        current_request.process_id == 0) {
        return new_request;
    }

    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],
                                          int *queue_cnt,
                                          int current_cylinder,
                                          int scan_direction) {
    struct RCB nullrcb = {0, 0, 0, 0, 0};

    if (*queue_cnt == 0) {
        return nullrcb;
    }

    int idx = -1;

    for (int i = 0; i < *queue_cnt; i++) {
        if (request_queue[i].cylinder == current_cylinder) {
            if (idx == -1 ||
                request_queue[i].arrival_timestamp <
                request_queue[idx].arrival_timestamp) {
                idx = i;
            }
        }
    }

    if (idx != -1) {
        struct RCB next = request_queue[idx];
        for (int i = idx; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
        return next;
    }

    if (scan_direction == 1) {
        int best_idx = -1;
        int best_delta = 0;

        for (int i = 0; i < *queue_cnt; i++) {
            if (request_queue[i].cylinder > current_cylinder) {
                int delta = request_queue[i].cylinder - current_cylinder;
                if (best_idx == -1 ||
                    delta < best_delta ||
                   (delta == best_delta &&
                    request_queue[i].arrival_timestamp <
                    request_queue[best_idx].arrival_timestamp)) {
                    best_idx = i;
                    best_delta = delta;
                }
            }
        }

        if (best_idx != -1) {
            idx = best_idx;
        } else {
            int closest_idx = 0;
            int best_dist = (request_queue[0].cylinder >= current_cylinder) ?
                            (request_queue[0].cylinder - current_cylinder) :
                            (current_cylinder - request_queue[0].cylinder);

            for (int i = 1; i < *queue_cnt; i++) {
                int cyl = request_queue[i].cylinder;
                int dist = (cyl >= current_cylinder) ?
                           (cyl - current_cylinder) :
                           (current_cylinder - cyl);

                if (dist < best_dist ||
                   (dist == best_dist &&
                    request_queue[i].arrival_timestamp <
                    request_queue[closest_idx].arrival_timestamp)) {
                    closest_idx = i;
                    best_dist = dist;
                }
            }
            idx = closest_idx;
        }
    } else { 

      int best_idx = -1;
        int best_delta = 0;

        for (int i = 0; i < *queue_cnt; i++) {
            if (request_queue[i].cylinder < current_cylinder) {
                int delta = current_cylinder - request_queue[i].cylinder;
                if (best_idx == -1 ||
                    delta < best_delta ||
                   (delta == best_delta &&
                    request_queue[i].arrival_timestamp <
                    request_queue[best_idx].arrival_timestamp)) {
                    best_idx = i;
                    best_delta = delta;
                }
            }
        }

        if (best_idx != -1) {
            idx = best_idx;
        } else {
            int closest_idx = 0;
            int best_dist = (request_queue[0].cylinder >= current_cylinder) ?
                            (request_queue[0].cylinder - current_cylinder) :
                            (current_cylinder - request_queue[0].cylinder);

            for (int i = 1; i < *queue_cnt; i++) {
                int cyl = request_queue[i].cylinder;
                int dist = (cyl >= current_cylinder) ?
                           (cyl - current_cylinder) :
                           (current_cylinder - cyl);

                if (dist < best_dist ||
                   (dist == best_dist &&
                    request_queue[i].arrival_timestamp <
                    request_queue[closest_idx].arrival_timestamp)) {
                    closest_idx = i;
                    best_dist = dist;
                }
            }
            idx = closest_idx;
        }
    }

    struct RCB next = request_queue[idx];
    for (int i = idx; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_cnt)--;

    return next;
}
