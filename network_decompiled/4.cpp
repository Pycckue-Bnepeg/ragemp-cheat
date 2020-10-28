
/* WARNING: Could not reconcile some variable overlaps */

void FUN_1400bd260(longlong param_1, BitStream *param_2)

{
    undefined8 *puVar1;
    code *pcVar2;
    void *pvVar3;
    byte bVar4;
    undefined8 uVar5;
    void *_Memory;
    undefined *puVar6;
    undefined8 *puVar7;
    undefined4 *puVar8;
    undefined8 *puVar9;
    uint uVar10;
    longlong *plVar11;
    undefined auStack136[32];
    undefined4 local_68;
    undefined2 local_58;
    undefined4 local_54;
    int local_50[2];
    undefined8 local_48;
    undefined local_40;
    undefined7 uStack63;
    undefined8 local_30;
    ulonglong local_28;
    ulonglong local_20;

    local_48 = 0xfffffffffffffffe;
    local_20 = DAT_1401727a0 ^ (ulonglong)auStack136;
    local_50[0] = (int)(*(longlong *)(param_1 + 0x60) - *(longlong *)(param_1 + 0x58) >> 3);
    WriteBits(param_2, (uchar *)local_50, 0x20, true);
    if (local_50[0] != 0)
    {
        WriteBits(param_2, *(uchar **)(param_1 + 0x58), local_50[0] << 6, true);
        puVar9 = *(undefined8 **)(param_1 + 0x70);
        puVar1 = *(undefined8 **)(param_1 + 0x78);
        while (puVar9 != puVar1)
        {
            bVar4 = *(byte *)((longlong)puVar9 + 0xc);
            if (7 < bVar4)
                goto LAB_1400bd6eb;
            local_58._1_1_ = (undefined)((ushort)local_58 >> 8);
            local_54._1_3_ = (uint3)(local_54 >> 8);
            switch (bVar4)
            {
            case 0:
                local_58 = CONCAT11(local_58._1_1_, 9);
                if (bVar4 == 0)
                {
                    local_54 = *(uint *)puVar9;
                }
                else
                {
                    local_54 = 0;
                }
                WriteBits(param_2, (uchar *)&local_58, 8, true);
                uVar10 = 0x20;
                puVar8 = &local_54;
                break;
            case 1:
                local_58 = CONCAT11(local_58._1_1_, 0xb);
                if (bVar4 == 1)
                {
                    local_54 = *(uint *)puVar9;
                }
                else
                {
                    local_54 = 0;
                }
                WriteBits(param_2, (uchar *)&local_58, 8, true);
                uVar10 = 0x20;
                puVar8 = &local_54;
                break;
            case 2:
                local_58 = CONCAT11(local_58._1_1_, 0xc);
                WriteBits(param_2, (uchar *)&local_58, 8, true);
                if ((*(char *)((longlong)puVar9 + 0xc) != '\x02') ||
                    (puVar6 = (undefined *)*puVar9, puVar6 == (undefined *)0x0))
                {
                    puVar6 = &DAT_140102e99;
                }
                local_30 = 0;
                local_28 = 0xf;
                local_40 = 0;
                plVar11 = (longlong *)0xffffffffffffffff;
                do
                {
                    plVar11 = (longlong *)((longlong)plVar11 + 1);
                } while (puVar6[(longlong)plVar11] != '\0');
                ReadFromConfig((longlong **)&local_40, puVar6, plVar11);
                local_54 = local_54 & 0xffff0000 | (uint)(ushort)local_30;
                WriteBits(param_2, (uchar *)&local_54, 0x10, true);
                puVar7 = &local_40;
                if (0xf < local_28)
                {
                    puVar7 = (undefined8 *)CONCAT71(uStack63, local_40);
                }
                WriteBits(param_2, (uchar *)puVar7, (int)local_30 * 8, true);
                if (0xf < local_28)
                {
                    pvVar3 = (void *)CONCAT71(uStack63, local_40);
                    _Memory = pvVar3;
                    if ((0xfff < local_28 + 1) &&
                        (_Memory = *(void **)((longlong)pvVar3 + -8),
                         0x1f < (ulonglong)((longlong)pvVar3 + (-8 - (longlong)_Memory))))
                    {
                        _invalid_parameter_noinfo_noreturn();
                        pcVar2 = (code *)swi(3);
                        (*pcVar2)();
                        return;
                    }
                    free(_Memory);
                }
                goto LAB_1400bd6eb;
            case 3:
                local_58 = CONCAT11(local_58._1_1_, 10);
                if (bVar4 == 3)
                {
                    bVar4 = *(byte *)puVar9;
                }
                else
                {
                    bVar4 = 0;
                }
                local_54 = local_54 & 0xffffff00 | (uint)bVar4;
                WriteBits(param_2, (uchar *)&local_58, 8, true);
            LAB_1400bd6d6:
                puVar8 = &local_54;
                goto LAB_1400bd6da;
            case 4:
                local_54 = CONCAT31(local_54._1_3_, 0xe);
                WriteBits(param_2, (uchar *)&local_54, 8, true);
                puVar8 = (undefined4 *)FUN_1400bfc40(puVar9);
                uVar10 = 0x60;
                break;
            case 5:
                local_58 = CONCAT11(local_58._1_1_, 0xd);
                WriteBits(param_2, (uchar *)&local_58, 8, true);
                if ((*(char *)((longlong)puVar9 + 0xc) != '\x05') ||
                    (puVar6 = (undefined *)*puVar9, puVar6 == (undefined *)0x0))
                {
                    puVar6 = &DAT_140102e99;
                }
                local_30 = 0;
                local_28 = 0xf;
                local_40 = 0;
                plVar11 = (longlong *)0xffffffffffffffff;
                do
                {
                    plVar11 = (longlong *)((longlong)plVar11 + 1);
                } while (puVar6[(longlong)plVar11] != '\0');
                ReadFromConfig((longlong **)&local_40, puVar6, plVar11);
                local_54 = local_54 & 0xffff0000 | (uint)(ushort)local_30;
                WriteBits(param_2, (uchar *)&local_54, 0x10, true);
                puVar7 = &local_40;
                if (0xf < local_28)
                {
                    puVar7 = (undefined8 *)CONCAT71(uStack63, local_40);
                }
                WriteBits(param_2, (uchar *)puVar7, (int)local_30 * 8, true);
                if (0xf < local_28)
                {
                    pvVar3 = (void *)CONCAT71(uStack63, local_40);
                    _Memory = pvVar3;
                    if ((0xfff < local_28 + 1) &&
                        (_Memory = *(void **)((longlong)pvVar3 + -8),
                         0x1f < (ulonglong)((longlong)pvVar3 + (-8 - (longlong)_Memory))))
                    {
                        _invalid_parameter_noinfo_noreturn();
                        pcVar2 = (code *)swi(3);
                        (*pcVar2)();
                        return;
                    }
                    free(_Memory);
                }
                goto LAB_1400bd6eb;
            case 6:
                local_58 = CONCAT11(local_58._1_1_, 0xf);
                puVar8 = &local_58;
            LAB_1400bd6da:
                uVar10 = 8;
                break;
            case 7:
                uVar5 = FUN_1400b98d0((byte *)puVar9);
                local_68 = 0;
                plVar11 = (longlong *)__RTDynamicCast(uVar5, 0, &class_rage::IEntity_RTTI_Type_Descriptor);
                if ((plVar11 == (longlong *)0x0) || (bVar4 = (**(code **)(*plVar11 + 8))(), 8 < bVar4))
                {
                    local_54 = CONCAT31(local_54._1_3_, 0xf);
                    goto LAB_1400bd6d6;
                }
                switch (bVar4)
                {
                case 0:
                    local_54 = (uint)local_54._1_3_ << 8;
                    if (*(char *)((longlong)puVar9 + 0xc) == '\a')
                    {
                        local_58 = *(undefined2 *)((longlong)puVar9 + 1);
                    }
                    else
                    {
                    LAB_1400bd610:
                        local_58 = 0xffff;
                    }
                    break;
                case 1:
                    local_54 = CONCAT31(local_54._1_3_, 1);
                    if (*(char *)((longlong)puVar9 + 0xc) != '\a')
                        goto LAB_1400bd610;
                    local_58 = *(undefined2 *)((longlong)puVar9 + 1);
                    break;
                case 2:
                    local_54 = CONCAT31(local_54._1_3_, 2);
                    if (*(char *)((longlong)puVar9 + 0xc) != '\a')
                        goto LAB_1400bd610;
                    local_58 = *(undefined2 *)((longlong)puVar9 + 1);
                    break;
                case 3:
                    local_54 = CONCAT31(local_54._1_3_, 3);
                    if (*(char *)((longlong)puVar9 + 0xc) != '\a')
                        goto LAB_1400bd610;
                    local_58 = *(undefined2 *)((longlong)puVar9 + 1);
                    break;
                case 4:
                    local_54 = CONCAT31(local_54._1_3_, 4);
                    if (*(char *)((longlong)puVar9 + 0xc) != '\a')
                        goto LAB_1400bd610;
                    local_58 = *(undefined2 *)((longlong)puVar9 + 1);
                    break;
                case 5:
                    local_54 = CONCAT31(local_54._1_3_, 5);
                    if (*(char *)((longlong)puVar9 + 0xc) != '\a')
                        goto LAB_1400bd610;
                    local_58 = *(undefined2 *)((longlong)puVar9 + 1);
                    break;
                case 6:
                    local_54 = CONCAT31(local_54._1_3_, 6);
                    if (*(char *)((longlong)puVar9 + 0xc) != '\a')
                        goto LAB_1400bd610;
                    local_58 = *(undefined2 *)((longlong)puVar9 + 1);
                    break;
                case 7:
                    local_54 = CONCAT31(local_54._1_3_, 7);
                    if (*(char *)((longlong)puVar9 + 0xc) != '\a')
                        goto LAB_1400bd610;
                    local_58 = *(undefined2 *)((longlong)puVar9 + 1);
                    break;
                case 8:
                    local_54 = CONCAT31(local_54._1_3_, 8);
                    if (*(char *)((longlong)puVar9 + 0xc) != '\a')
                        goto LAB_1400bd610;
                    local_58 = *(undefined2 *)((longlong)puVar9 + 1);
                }
                WriteBits(param_2, (uchar *)&local_54, 8, true);
                uVar10 = 0x10;
                puVar8 = &local_58;
            }
            WriteBits(param_2, (uchar *)puVar8, uVar10, true);
        LAB_1400bd6eb:
            puVar9 = (undefined8 *)((longlong)puVar9 + 0xd);
        }
    }
    FUN_1400f3c90(local_20 ^ (ulonglong)auStack136);
    return;
}

struct struct_
{
};

struct struct_unk
{
};

void someshit()
{
    auto some_struct = (struct_ *)0xFFFFFFF;
    auto ptr_unk = (struct_unk *)(some_struct[0x28]);

    auto some_value = *ptr_unk;

    while (some_value != ptr_unk)
    {
        uint16_t value = some_value[4]; // length of below
        WriteBits(out, value, 0x10, true);
    }

    puVar3 = *(undefined8 **)(param_1 + 0x28);
    puVar1 = (undefined8 *)*puVar3;
    while (puVar1 != puVar3)
    {
        local_res8 = local_res8 & 0xffff0000 | (uint) * (ushort *)(puVar1 + 4);
        WriteBits(this, (uchar *)&local_res8, 0x10, true);
        puVar2 = puVar1 + 2;
        if (0xf < (ulonglong)puVar1[5])
        {
            puVar2 = (undefined8 *)puVar1[2];
        }
        WriteBits(this, (uchar *)puVar2, (int)puVar1[4] * 8, true);
        if (*(char *)(puVar1 + 6) == '\0')
        {
            Write0(this);
        }
        else
        {
            Write1(this);
        }
        puVar1 = (undefined8 *)*puVar1;
    }
}
