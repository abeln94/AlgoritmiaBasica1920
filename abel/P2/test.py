from queue import PriorityQueue


def dist(x, y, xp, yp):
    return abs(xp - x) + abs(yp - y)


def cost(chosed):
    cost = 0

    xp, yp = x0, y0
    for x, y in chosed:
        cost += dist(x, y, xp, yp)
        xp, yp = x, y

    cost += dist(xp, yp, x0, y0)

    return cost


with open('prueba.txt') as input:
    N = int(next(input))
    with open('sol_prueba.txt') as output:
        for n in range(N):
            A, B = [int(x) for x in next(input).split()]
            x0, y0 = [int(x) for x in next(input).split()]
            M = int(next(input))
            SOL, _ = [x for x in next(output).split()]
            SOL = int(SOL)

            mines = []
            for m in range(M):
                mine = [int(x) for x in next(input).split()]
                if mine not in mines and mine != (x0, y0): mines.append(mine)

            best = None
            bestcost = float("inf")

            nodes = PriorityQueue()
            nodes.put((0, ([], mines)))
            while not nodes.empty():
                thecost, (chosed, available) = nodes.get()

                if thecost >= bestcost: continue

                if not available and thecost < bestcost:
                    bestcost = thecost
                    best = chosed

                for mine in available:
                    newchosed = chosed.copy()
                    newchosed.append(mine)
                    newavailable = available.copy()
                    newavailable.remove(mine)
                    nodes.put((cost(newchosed), (newchosed, newavailable)))

            if bestcost != SOL:
                print()
            else:
                print(".")
            print("CORRECTO:" if bestcost != SOL else "ERROR:", f"se esperaba {SOL} y se ha obtenido {bestcost} {best}")
