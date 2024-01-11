import math
from icecream import ic


def parallel(Ra, Rb):
    return (Ra * Rb) / (Ra + Rb)


Vref = 4.7
R1 = 1000
R3 = 1000
R4 = parallel(1000, 10000)

V = 13
# Rn = 13
# Rp = 13


def test():
    Uref = 1.235
    R2 = 1500.0
    I = Uref / R2

    Rm = 1000.0
    Um = Rm * I

    print(f"Um = {Um}V")


    Vref = 1.235
    value = 686
    # value = 597.0

    Vin = value * Vref / 1024.0
    Rm = Vin / I

    print(f"Vin = {Vin}V ({2 * Vin})")
    print(f"Rm = {Rm}")

    print(f"1 LSB = {Vref / 1024}")


def main():
    value = 745

    Vref = 1.235
    R2 = 1500
    I = Vref / R2
    Vin = value * Vref / 1024
    Rm = Vin / I
    T = 3405.705 - math.sqrt(11598824 - 1743.2 * (Rm - 1000))

    T = 3383.8 * (1 - math.sqrt(1 - 0.15123 * (Rm / 1000 - 1)))

    k = 3.85  # Ohm/Grad
    print(f"{value} -> Rm = {Rm} Ohm -> T = {T}")


def U2D(U):
    value = 1024 * U / Vref
    return round(value)


def D2U(value):
    return Vref * value / 1024


def R2U(Rm):
    Ue = Vref
    Um = Ue * Rm / (R1 + Rm)
    U4 = Ue * R4 / (R3 + R4)
    Ua = V * (Um - U4)
    return Ua


def U2R(Ua):
    Ue = Vref
    U4 = Ue * R4 / (R3 + R4)
    Um = Ua / V + U4
    # Um (R1 + Rm) = Ue * Rm ==> Ue * Rm - Um * Rm = Um * R1 ==>
    Rm = Um * R1 / (Ue - Um)
    return Rm


def T2R(T):
    R0 = 1000
    A = 3.9083 * 1E-3  # (°C-1)
    B = -5.7750 * 1E-7  # (°C-2)
    C = -4.1830 * 1E-12  # (°C-4

    if T < 0:
        Rt = R0 * (1 + A * T + B * T * T + C * (T - 100) * T * T * T)
    else:
        Rt = R0 * (1 + A * T + B * T * T)
    return Rt


def R2T(Rt):
    R0 = 1000
    A = 3.9083 * 1E-3  # (°C-1)
    B = -5.7750 * 1E-7  # (°C-2)
    C = -4.1830 * 1E-12  # (°C-4
    # Rt / R0 = B * T * T + A * T + 1
    # B * T * T + A * T + 1 - Rt / R0 = 0
    # T * T + A * T / B + (1 - Rt / R0) / B = 0
    p = A / B
    q = (1 - Rt / R0) / B
    s = math.sqrt((p / 2) ** 2 - q)
    T1 = -p / 2 + s
    T2 = -p / 2 - s
    return T2, T1


def ApproxT2R(T):
    R0 = 1000
    A = 3.9083 * 1E-3  # (°C-1)
    B = -5.7750 * 1E-7  # (°C-2)
    C = -4.1830 * 1E-12  # (°C-4

    Rt = R0 * (1 + A * T + B * T * T)
    return Rt


def T2D(T):
    R = T2R(T)
    U = R2U(R)
    D = U2D(U)

    Rapprox = ApproxT2R(T)
    Uapprox = R2U(Rapprox)
    Dapprox = U2D(Uapprox)
    # ic(T, R, U, D)
    print(f"{T:3}  {D:5}  {Dapprox:5}")


def D2T(D):
    U = D2U(D)
    R = U2R(U)
    T1, T2 = R2T(R)
    return T1


def D2T_(D):
    k = R4 / (R3 + R4)
    R = R1 * (D + 1024 * V * k) / (1024 * V * (1 - k) - D)
    T1, T2 = R2T(R)
    return T1



if __name__ == "__main__":
    #test()
    #main()

    vmin = R2U(921.6) / V
    vmax =R2U(1232.4) / V
    ic(vmin, vmax)
    vmin = R2U(921.6)
    vmax =R2U(1232.4)
    ic(vmin, vmax)


    [T2D(T) for T in [-20, 0, 20, 40, 60]]


    for D in [45, 317, 567, 1010]:
        T = D2T(D)
        print(f"{D:4} {T:7.1f}")
        T = D2T_(D)
        print(f"{D:4} {T:7.1f}")

    with open("_.txt", "w") as fh:
        d = ["D                        "]
        t = ["T/$\\unit{\degreeCelsius}$"]
        for D in range(1024):
            if divmod(D, 8)[1] == 0:
                print(" & ".join(d), file=fh, end=" \\\\\n")
                print(" & ".join(t), file=fh, end=" \\\\\n")
                d = ["D                        "]
                t = ["T/$\\unit{\\degreeCelsius}$"]
            T = D2T(D)
            d.append(f"{D:6}")
            t.append(f"{T:6.2f}")
        print(" & ".join(d), file=fh, end=" \\\\\n")
        print(" & ".join(t), file=fh, end=" \\\\\n")

#    [D2T(D) for D in range(1024)]

