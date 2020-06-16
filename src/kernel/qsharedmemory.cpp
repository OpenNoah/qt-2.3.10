/****************************************************************************
** $Id: qt/src/kernel/qsharedmemory.cpp   2.3.10   edited 2005-01-24 $
**
** Implementation of Qt/Embedded shared memory class
**
** Created : 000101
**
** Copyright (C) 2000-2002 Trolltech AS.  All rights reserved.
**
** This file is part of the kernel module of the Qt GUI Toolkit.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses for Qt/Embedded may use this file in accordance with the
** Qt Embedded Commercial License Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include <qsharedmemory.h>

#if !defined(QT_QWS_NO_SHM)

#include <sys/ipc.h>
#include <sys/types.h>

#if defined(QT_POSIX_QSHM)
#include <fcntl.h>
#include <sys/mman.h>

QSharedMemory::QSharedMemory (int size, QString filename)
{
  shmSize = size;
  shmFile = filename;
}

bool QSharedMemory::create ()
{
  shmFD = shm_open (shmFile.latin1 (), O_RDWR | O_EXCL | O_CREAT, 0666);
  if (shmFD == -1)
    return FALSE;
  else if (ftruncate (shmFD, shmSize) == -1)
    {
      close (shmFD);
      return FALSE;
    }

  return TRUE;
}

void
QSharedMemory::destroy ()
{
  shm_unlink (shmFile.latin1 ());
}

bool QSharedMemory::attach ()
{
  shmBase = mmap (0, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmFD, 0);

  if (shmBase == MAP_FAILED)
    return FALSE;

  close (shmFD);
  return TRUE;
}

void
QSharedMemory::detach ()
{
  munmap (shmBase, shmSize);
}

void
QSharedMemory::setPermissions (mode_t mode)
{
  mprotect (shmBase, shmSize, mode);	// Provide defines to make prot work properly
}

#else // Assume SysV for backwards compat
#include <sys/shm.h>

QSharedMemory::QSharedMemory (int size, QString filename)
{
  shmSize = size;
  shmFile = filename;
  key = ftok (shmFile.latin1 (), 'Q');
  idInitted = FALSE;
}

bool QSharedMemory::create ()
{
  shmId = shmget (key, shmSize, IPC_CREAT | 0666);
  if (shmId == -1)
    return FALSE;
  else
    return TRUE;
}

void
QSharedMemory::destroy ()
{
  struct shmid_ds shm;
  shmctl (shmId, IPC_RMID, &shm);
}

bool QSharedMemory::attach ()
{
  if (shmId == -1)
    shmId = shmget (key, shmSize, 0);

  shmBase = shmat (shmId, 0, 0);
  if ((int) shmBase == -1 || shmBase == 0)
    return FALSE;
  else
    return TRUE;
}

void
QSharedMemory::detach ()
{
  shmdt ((char*)shmBase);
}

void
QSharedMemory::setPermissions (mode_t mode)
{
  struct shmid_ds shm;
  shmctl (shmId, IPC_STAT, &shm);
  shm.shm_perm.mode = mode;
  shmctl (shmId, IPC_SET, &shm);
}

#endif

#endif