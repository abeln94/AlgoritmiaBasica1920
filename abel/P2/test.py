def dist(p1, p2):
    return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])


def _cost(secuence):
    cost = 0

    prev = initial
    for c in secuence:
        cost += dist(c, prev)
        prev = c

    cost += dist(prev, initial)

    return cost


def cost(chosed, available):
    if len(available) == 0:
        cost = _cost(chosed)
        return cost, cost

    last = chosed[-1] if len(chosed) > 0 else initial
    far = max(available, key=lambda p: dist(p, last))
    mincost = _cost(chosed + [far])

    maxcost = mincost

    for c in [c for c in available if c != far]:
        maxcost += _cost([c])

    return mincost, maxcost


with open('prueba.txt') as input:
    N = int(next(input))
    with open('sol_prueba.txt') as output:
        for n in range(N):
            A, B = [int(x) for x in next(input).split()]
            initial = [int(x) for x in next(input).split()]
            M = int(next(input))
            SOL, _ = [x for x in next(output).split()]
            SOL = int(SOL)

            mines = []
            for m in range(M):
                mine = [int(x) for x in next(input).split()]
                if mine not in mines and mine != initial: mines.append(mine)

            best = None
            bestcost = float("inf")

            firstcost, cota = cost([], mines)

            nodes = [(firstcost, ([], mines))]
            while len(nodes) != 0:
                thecost, (chosed, available) = nodes.pop()

                # poda
                if thecost > cota: continue

                # update solution if better
                if not available and thecost < bestcost:
                    bestcost = thecost
                    best = chosed

                nextnodes = []
                for mine in available:
                    newchosed = chosed.copy()
                    newchosed.append(mine)
                    newavailable = available.copy()
                    newavailable.remove(mine)

                    newcost, newcota = cost(newchosed, newavailable)

                    if newcota < cota: cota = newcota

                    nextnodes.append((newcost, (newchosed, newavailable)))
                nextnodes.sort(key=lambda x: x[0], reverse=True)
                nodes.extend(nextnodes)

            print("CORRECTO:" if bestcost == SOL else "ERROR:", f"se esperaba {SOL} y se ha obtenido {bestcost} {best}")
