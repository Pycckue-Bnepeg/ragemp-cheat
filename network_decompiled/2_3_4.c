
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Could not reconcile some variable overlaps */

void FUN_1400d99c0(undefined8 param_1, undefined8 param_2, BitStream *param_3, ushort param_4)

{
    SystemAddress *this;
    undefined4 *puVar1;
    undefined4 *puVar2;
    undefined4 *puVar3;
    longlong *plVar4;
    longlong lVar5;
    void *pvVar6;
    ushort uVar7;
    bool bVar8;
    int iVar9;
    undefined8 *extraout_RAX;
    longlong *plVar10;
    SystemAddress *pSVar11;
    undefined8 extraout_RAX_00;
    undefined *puVar12;
    undefined8 uVar13;
    longlong **pplVar14;
    longlong extraout_RAX_01;
    undefined8 *extraout_RAX_02;
    longlong lVar15;
    void **ppvVar16;
    size_t sVar17;
    undefined8 *_Memory;
    ulonglong uVar18;
    void *_Dst;
    uchar *puVar19;
    int iVar20;
    ulonglong _Size;
    undefined8 *puVar21;
    longlong **pplVar22;
    undefined8 *_Dst_00;
    longlong **pplVar23;
    undefined8 *puVar24;
    code **ppcVar25;
    longlong in_GS_OFFSET;
    undefined4 extraout_XMM0_Da;
    undefined4 extraout_XMM0_Da_00;
    undefined4 extraout_XMM0_Da_01;
    undefined4 extraout_XMM0_Db;
    undefined4 extraout_XMM0_Db_00;
    undefined4 extraout_XMM0_Db_01;
    RakNetGUID *local_2d0;
    undefined8 local_2c8;
    uint local_2c0;
    undefined4 local_2b8;
    ushort local_2a8[4];
    ushort local_2a0[4];
    undefined2 local_298[4];
    void **local_290;
    longlong *local_288;
    undefined4 local_280;
    undefined4 uStack636;
    undefined4 local_278;
    undefined4 uStack628;
    RakNetGUID local_270[8];
    undefined2 local_268;
    SystemAddress local_260[24];
    BitStream *local_248;
    undefined *local_240;
    undefined8 local_238;
    void **local_230;
    void *local_228[4];
    void *local_208[4];
    uchar local_1e8;
    undefined7 uStack487;
    ulonglong local_1d8;
    ulonglong local_1d0;
    uchar local_1c8;
    undefined7 uStack455;
    ulonglong local_1b8;
    ulonglong local_1b0;
    uchar local_1a8;
    undefined7 uStack423;
    ulonglong local_198;
    ulonglong local_190;
    void *local_188[4];
    BitStream local_168[288];
    ulonglong local_48;

    local_238 = 0xfffffffffffffffe;
    local_48 = DAT_1401727a0 ^ (ulonglong)&stack0xfffffffffffffd08;
    puVar21 = (undefined8 *)0x0;
    _Memory = DAT_140175518;
    local_2a0[0] = param_4;
    if ((DAT_140175518 == (undefined8 *)0x0) &&
        (local_290 = (void **)CustomAlloc(0x18), _Memory = puVar21, local_290 != (void **)0x0))
    {
        M_StartServer(CONCAT44(extraout_XMM0_Db, extraout_XMM0_Da), param_2, (longlong *)local_290);
        _Memory = extraout_RAX;
    }
    plVar4 = *(longlong **)(_Memory[1] + 0x10);
    (**(code **)(*plVar4 + 0x120))(plVar4, &local_280, (ulonglong)local_2a0[0]);
    plVar10 = (longlong *)FUN_1400896c0();
    if ((local_2a0[0] < *(ushort *)((longlong)plVar10 + 10)) &&
        (lVar15 = *(longlong *)(*plVar10 + (ulonglong)local_2a0[0] * 8), lVar15 != 0))
    {
        this = (SystemAddress *)(lVar15 + 0x2b4);
        pSVar11 = (SystemAddress *)(**(code **)(*plVar4 + 0x118))(plVar4, local_188, (ulonglong)local_2a0[0]);
        bVar8 = operator==(this, pSVar11);
        if (bVar8 != false)
        {
            lVar15 = *plVar4;
            AddressOrGUID((AddressOrGUID *)local_270, this);
            (**(code **)(lVar15 + 0xf8))(plVar4, extraout_RAX_00, 0);
        }
        plVar10 = (longlong *)FUN_1400896c0();
        if (local_2a0[0] < *(ushort *)((longlong)plVar10 + 10))
        {
            uVar18 = (ulonglong)local_2a0[0];
            lVar15 = *plVar10;
            if (*(longlong *)(lVar15 + uVar18 * 8) != 0)
            {
                if (local_2a0[0] <= *(ushort *)(plVar10 + 1) && *(ushort *)(plVar10 + 1) != local_2a0[0])
                {
                    *(ushort *)(plVar10 + 1) = local_2a0[0];
                }
                _Memory = *(undefined8 **)(lVar15 + uVar18 * 8);
                if (_Memory != (undefined8 *)0x0)
                {
                    FUN_140088290(_Memory);
                    free(_Memory);
                    lVar15 = *plVar10;
                }
                *(undefined8 *)(lVar15 + uVar18 * 8) = 0;
                *(short *)((longlong)plVar10 + 0xc) = *(short *)((longlong)plVar10 + 0xc) + -1;
            }
        }
    }
    local_1d8 = 0;
    local_1d0 = 0xf;
    local_1e8 = '\0';
    local_2a8[0] = 0;
    /* nickname and its length */
    bVar8 = ReadBits(param_3, (uchar *)local_2a8, 0x10, true);
    if (bVar8 != false)
    {
        uVar18 = (ulonglong)local_2a8[0];
        if (local_1d8 < uVar18)
        {
            _Size = uVar18 - local_1d8;
            if (local_1d0 - local_1d8 < _Size)
            {
                FUN_140002dc0((void **)&local_1e8, _Size, 0, _Size, '\0');
            }
            else
            {
                puVar12 = (undefined *)&local_1e8;
                if (0xf < local_1d0)
                {
                    puVar12 = (undefined *)CONCAT71(uStack487, local_1e8);
                }
                _Dst = puVar12 + local_1d8;
                local_1d8 = uVar18;
                memset(_Dst, 0, _Size);
                *(undefined *)((longlong)_Dst + _Size) = 0;
            }
        }
        else
        {
            puVar12 = (undefined *)&local_1e8;
            if (0xf < local_1d0)
            {
                puVar12 = (undefined *)CONCAT71(uStack487, local_1e8);
            }
            local_1d8 = uVar18;
            puVar12[uVar18] = 0;
        }
        puVar19 = (uchar *)&local_1e8;
        if (0xf < local_1d0)
        {
            puVar19 = (uchar *)CONCAT71(uStack487, local_1e8);
        }
        ReadBits(param_3, puVar19, (uint)local_2a8[0] << 3, true);
    }
    local_198 = 0;
    local_190 = 0xf;
    local_1a8 = '\0';
    local_2a8[0] = 0;
    /* serial and its length */
    bVar8 = ReadBits(param_3, (uchar *)local_2a8, 0x10, true);
    if (bVar8 != false)
    {
        uVar18 = (ulonglong)local_2a8[0];
        if (local_198 < uVar18)
        {
            _Size = uVar18 - local_198;
            if (local_190 - local_198 < _Size)
            {
                FUN_140002dc0((void **)&local_1a8, _Size, 0, _Size, '\0');
            }
            else
            {
                puVar12 = (undefined *)&local_1a8;
                if (0xf < local_190)
                {
                    puVar12 = (undefined *)CONCAT71(uStack423, local_1a8);
                }
                _Dst = puVar12 + local_198;
                local_198 = uVar18;
                memset(_Dst, 0, _Size);
                *(undefined *)((longlong)_Dst + _Size) = 0;
            }
        }
        else
        {
            puVar12 = (undefined *)&local_1a8;
            if (0xf < local_190)
            {
                puVar12 = (undefined *)CONCAT71(uStack423, local_1a8);
            }
            local_198 = uVar18;
            puVar12[uVar18] = 0;
        }
        puVar19 = (uchar *)&local_1a8;
        if (0xf < local_190)
        {
            puVar19 = (uchar *)CONCAT71(uStack423, local_1a8);
        }
        ReadBits(param_3, puVar19, (uint)local_2a8[0] << 3, true);
    }
    local_1b8 = 0;
    local_1b0 = 0xf;
    local_1c8 = '\0';
    local_2a8[0] = 0;
    /* socialclub nick and its length
                        */
    bVar8 = ReadBits(param_3, (uchar *)local_2a8, 0x10, true);
    if (bVar8 != false)
    {
        uVar18 = (ulonglong)local_2a8[0];
        if (local_1b8 < uVar18)
        {
            _Size = uVar18 - local_1b8;
            if (local_1b0 - local_1b8 < _Size)
            {
                FUN_140002dc0((void **)&local_1c8, _Size, 0, _Size, '\0');
            }
            else
            {
                puVar12 = (undefined *)&local_1c8;
                if (0xf < local_1b0)
                {
                    puVar12 = (undefined *)CONCAT71(uStack455, local_1c8);
                }
                _Dst = puVar12 + local_1b8;
                local_1b8 = uVar18;
                memset(_Dst, 0, _Size);
                *(undefined *)((longlong)_Dst + _Size) = 0;
            }
        }
        else
        {
            puVar12 = (undefined *)&local_1c8;
            if (0xf < local_1b0)
            {
                puVar12 = (undefined *)CONCAT71(uStack455, local_1c8);
            }
            local_1b8 = uVar18;
            puVar12[uVar18] = 0;
        }
        puVar19 = (uchar *)&local_1c8;
        if (0xf < local_1b0)
        {
            puVar19 = (uchar *)CONCAT71(uStack455, local_1c8);
        }
        ReadBits(param_3, puVar19, (uint)local_2a8[0] << 3, true);
    }
    local_290 = local_188;
    local_230 = local_228;
    uVar13 = FUN_1400896c0();
    pplVar22 = (longlong **)FUN_140003380(local_188, &local_1a8);
    pplVar23 = (longlong **)FUN_140003380(local_228, &local_1c8);
    pplVar14 = (longlong **)FUN_140003380(local_208, &local_1e8);
    FUN_1400dbd70(CONCAT44(extraout_XMM0_Db_00, extraout_XMM0_Da_00), param_2, uVar13, local_2a0[0],
                  pplVar14, pplVar23, pplVar22);

    BitStream(local_168);
    WriteRpcHeader(local_168, 2);
    WriteBits(local_168, (uchar *)local_2a0, 0x10, true);
    local_2a8[0] = (ushort)local_1d8;
    WriteBits(local_168, (uchar *)local_2a8, 0x10, true);
    puVar19 = (uchar *)&local_1e8;
    if (0xf < local_1d0)
    {
        puVar19 = (uchar *)CONCAT71(uStack487, local_1e8);
    }
    WriteBits(local_168, puVar19, (int)local_1d8 * 8, true);
    RakNetGUID(local_270);
    SystemAddress(local_260);
    local_268 = (undefined2)local_278;
    operator=(local_260, (SystemAddress *)&DAT_14017a6f8);
    local_2c0 = 0;
    local_2c8 = (RakNetGUID *)CONCAT71(local_2c8._1_7_, 1);
    local_2d0 = local_270;
    (**(code **)(*plVar4 + 0xa8))(plVar4, local_168, 2);
    Reset(local_168);

    WriteRpcHeader(local_168, 3);
    _Memory = DAT_140175518;
    if ((DAT_140175518 == (undefined8 *)0x0) &&
        (local_288 = (longlong *)CustomAlloc(0x18), _Memory = puVar21, local_288 != (longlong *)0x0))
    {
        M_StartServer(CONCAT44(extraout_XMM0_Db_01, extraout_XMM0_Da_01), param_2, local_288);
        _Memory = extraout_RAX_02;
    }
    lVar15 = _Memory[2];
    WriteBits(local_168, (uchar *)local_2a0, 0x10, true);
    lVar5 = *(longlong *)(*(longlong *)(in_GS_OFFSET + 0x58) + (ulonglong)DAT_14017bb28 * 8);
    if ((*(int *)(lVar5 + 4) < DAT_1401753f0) &&
        (_Init_thread_header(&DAT_1401753f0), DAT_1401753f0 == -1))
    {
        FUN_1400d19e0(&DAT_140175360);
        atexit(&LAB_1400ff330);
        _Init_thread_footer(&DAT_1401753f0);
    }
    FUN_1400d5fb0((longlong)&DAT_140175368, local_168);
    local_290 = (void **)((ulonglong)local_290 & 0xffffffff00000000 |
                          (ulonglong) * (uint *)(lVar15 + 0x134));
    local_2a8[0] = *(ushort *)(lVar15 + 0x130);
    local_298[0] = *(undefined2 *)(lVar15 + 0x132);
    WriteBits(local_168, (uchar *)&local_290, 0x20, true);
    WriteBits(local_168, (uchar *)local_2a8, 0x10, true);
    WriteBits(local_168, (uchar *)local_298, 0x10, true);
    local_2a8[0] = local_2a8[0] & 0xff00 | (ushort) * (byte *)(lVar15 + 0x138);
    WriteBits(local_168, (uchar *)local_2a8, 8, true);
    if ((*(int *)(lVar5 + 4) < DAT_14017a830) &&
        (_Init_thread_header(&DAT_14017a830), DAT_14017a830 == -1))
    {
        BitStream((BitStream *)&DAT_14017a710);
        atexit(&LAB_1400ff6a0);
        _Init_thread_footer(&DAT_14017a830);
    }
    Reset((BitStream *)&DAT_14017a710);
    WriteRpcHeader((BitStream *)&DAT_14017a710, 4);
    lVar15 = FUN_1400896c0();
    local_298[0] = *(undefined2 *)(lVar15 + 0xc);
    WriteBits((BitStream *)&DAT_14017a710, (uchar *)local_298, 0x10, true);
    pplVar14 = (longlong **)FUN_1400896c0();
    pplVar22 = (longlong **)*pplVar14;
    iVar20 = (uint) * (ushort *)((longlong)pplVar14 + 10) - 1;
    pplVar23 = pplVar22;
    if (*pplVar22 == (longlong *)0x0)
    {
        do
        {
            pplVar23 = pplVar23 + 1;
            if (*pplVar23 != (longlong *)0x0)
                break;
            iVar20 = iVar20 + -1;
        } while (iVar20 != -1);
    }
    pplVar22 = pplVar22 + *(ushort *)((longlong)pplVar14 + 10);
    iVar9 = -1;
    if (*pplVar22 == (longlong *)0x0)
    {
        do
        {
            pplVar22 = pplVar22 + 1;
            if (*pplVar22 != (longlong *)0x0)
                break;
            iVar9 = iVar9 + -1;
        } while (iVar9 != -1);
    }
LAB_1400da0c3:
    if (iVar20 != -1 && pplVar23 != pplVar22)
    {
        FUN_14008b850(*pplVar23, (BitStream *)&DAT_14017a710);
        do
        {
            pplVar23 = pplVar23 + 1;
            if (*pplVar23 != (longlong *)0x0)
                break;
            iVar20 = iVar20 + -1;
        } while (iVar20 != -1);
        goto LAB_1400da0c3;
    }
    local_248 = local_168;
    local_240 = &DAT_14017a710;
    if (DAT_14017a6f0 == DAT_14017a6e8)
    {
        FUN_1400dc080((void **)&DAT_14017a6e0, DAT_14017a6e8, &local_280);
    }
    else
    {
        puVar3 = DAT_14017a6e8 + 3;
        puVar2 = DAT_14017a6e8 + 2;
        puVar1 = DAT_14017a6e8 + 1;
        *DAT_14017a6e8 = local_280;
        *puVar1 = uStack636;
        *puVar2 = local_278;
        *puVar3 = uStack628;
        DAT_14017a6e8 = DAT_14017a6e8 + 4;
    }
    RakNetGUID(local_270);
    SystemAddress(local_260);
    local_268 = (undefined2)local_278;
    operator=(local_260, (SystemAddress *)&DAT_14017a6f8);
    local_2b8 = 0;
    local_2c0 = local_2c0 & 0xffffff00;
    local_2c8 = local_270;
    local_2d0 = (RakNetGUID *)((ulonglong)local_2d0 & 0xffffffffffffff00);
    (**(code **)(*plVar4 + 0xa0))(plVar4, &local_248);
    ppvVar16 = (void **)FUN_1400896c0();
    uVar7 = local_2a0[0];
    if (extraout_RAX_01 == 0)
    {
        _wassert(L"entity", L"r:\\ragemp\\source code\\shared\\pool.hpp", 0x119);
    }
    *(short *)((longlong)ppvVar16 + 0xc) = *(short *)((longlong)ppvVar16 + 0xc) + 1;
    if (uVar7 < *(ushort *)((longlong)ppvVar16 + 10))
    {
        *(longlong *)((longlong)*ppvVar16 + (ulonglong)uVar7 * 8) = extraout_RAX_01;
    }
    else
    {
        uVar18 = (ulonglong)(ushort)(uVar7 + 100);
        sVar17 = SUB168(ZEXT816(8) * ZEXT816(uVar18), 0);
        if (SUB168(ZEXT816(8) * ZEXT816(uVar18) >> 0x40, 0) != 0)
        {
            sVar17 = 0xffffffffffffffff;
        }
        _Dst_00 = (undefined8 *)CustomAlloc(sVar17);
        _Memory = (undefined8 *)(uVar18 * 8 + 7 >> 3);
        if (_Dst_00 + uVar18 < _Dst_00)
        {
            _Memory = puVar21;
        }
        puVar24 = _Dst_00;
        if (_Memory != (undefined8 *)0x0)
        {
            while (_Memory != (undefined8 *)0x0)
            {
                _Memory = (undefined8 *)((longlong)_Memory + -1);
                *puVar24 = 0;
                puVar24 = puVar24 + 1;
            }
        }
        memcpy(_Dst_00, *ppvVar16, (ulonglong) * (ushort *)((longlong)ppvVar16 + 10) << 3);
        free(*ppvVar16);
        *(undefined8 **)ppvVar16 = _Dst_00;
        *(ushort *)((longlong)ppvVar16 + 10) = uVar7 + 100;
        _Dst_00[uVar7] = extraout_RAX_01;
    }
    _Memory = DAT_14017a570;
    if ((DAT_14017a570 == (undefined8 *)0x0) &&
        (local_288 = (longlong *)CustomAlloc(0x108), _Memory = puVar21, local_288 != (longlong *)0x0))
    {
        _Memory = FUN_1400c9bb0(local_288);
    }
    ppcVar25 = (code **)_Memory[4];
    _Dst_00 = (undefined8 *)((ulonglong)((longlong)(code **)_Memory[5] + (7 - (longlong)ppcVar25)) >> 3);
    if ((code **)_Memory[5] < ppcVar25)
    {
        _Dst_00 = puVar21;
    }
    if (_Dst_00 != (undefined8 *)0x0)
    {
        do
        {
            (***(code ***)*ppcVar25)(*ppcVar25, extraout_RAX_01);
            ppcVar25 = ppcVar25 + 1;
            puVar21 = (undefined8 *)((longlong)puVar21 + 1);
        } while (puVar21 != _Dst_00);
    }
    ~BitStream(local_168);
    if (local_1b0 < 0x10)
    {
    LAB_1400da30f:
        local_1b8 = 0;
        local_1b0 = 0xf;
        local_1c8 = '\0';
        if (0xf < local_190)
        {
            pvVar6 = (void *)CONCAT71(uStack423, local_1a8);
            _Dst = pvVar6;
            if ((0xfff < local_190 + 1) &&
                (_Dst = *(void **)((longlong)pvVar6 + -8),
                 0x1f < (ulonglong)((longlong)pvVar6 + (-8 - (longlong)_Dst))))
                goto LAB_1400da398;
            free(_Dst);
        }
        local_198 = 0;
        local_190 = 0xf;
        local_1a8 = '\0';
        if (local_1d0 < 0x10)
            goto LAB_1400da3a4;
        pvVar6 = (void *)CONCAT71(uStack487, local_1e8);
        _Dst = pvVar6;
        if ((0xfff < local_1d0 + 1) &&
            (_Dst = *(void **)((longlong)pvVar6 + -8),
             0x1f < (ulonglong)((longlong)pvVar6 + (-8 - (longlong)_Dst))))
            goto LAB_1400da398;
    }
    else
    {
        pvVar6 = (void *)CONCAT71(uStack455, local_1c8);
        _Dst = pvVar6;
        if ((local_1b0 + 1 < 0x1000) ||
            (_Dst = *(void **)((longlong)pvVar6 + -8),
             (ulonglong)((longlong)pvVar6 + (-8 - (longlong)_Dst)) < 0x20))
        {
            free(_Dst);
            goto LAB_1400da30f;
        }
    LAB_1400da398:
        _invalid_parameter_noinfo_noreturn();
    }
    free(_Dst);
LAB_1400da3a4:
    FUN_1400f3c90(local_48 ^ (ulonglong)&stack0xfffffffffffffd08);
    return;
}
