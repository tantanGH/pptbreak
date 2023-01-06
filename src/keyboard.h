#ifndef __H_KEYBOARD__
#define __H_KEYBOARD__

#define KEY_GRP_ESC    (0)
#define KEY_GRP_CR     (3)
#define KEY_GRP_SPACE  (6)
#define KEY_GRP_LEFT   (7)
#define KEY_GRP_UP     (7)
#define KEY_GRP_RIGHT  (7)
#define KEY_GRP_DOWN   (7)
#define KEY_GRP_ENTER  (9)

#define KEY_BIT_ESC    (1)
#define KEY_BIT_CR     (5)
#define KEY_BIT_SPACE  (5)
#define KEY_BIT_LEFT   (3)
#define KEY_BIT_UP     (4)
#define KEY_BIT_RIGHT  (5)
#define KEY_BIT_DOWN   (6)
#define KEY_BIT_ENTER  (6)

#define KEY_SCAN_CODE_ESC    ( KEY_GRP_ESC   * 8 + KEY_BIT_ESC   )
#define KEY_SCAN_CODE_CR     ( KEY_GRP_CR    * 8 + KEY_BIT_CR    )
#define KEY_SCAN_CODE_SPACE  ( KEY_GRP_SPACE * 8 + KEY_BIT_SPACE )
#define KEY_SCAN_CODE_LEFT   ( KEY_GRP_LEFT  * 8 + KEY_BIT_LEFT  )
#define KEY_SCAN_CODE_UP     ( KEY_GRP_UP    * 8 + KEY_BIT_UP    )
#define KEY_SCAN_CODE_RIGHT  ( KEY_GRP_RIGHT * 8 + KEY_BIT_RIGHT )
#define KEY_SCAN_CODE_DOWN   ( KEY_GRP_DOWN  * 8 + KEY_BIT_DOWN  )
#define KEY_SCAN_CODE_ENTER  ( KEY_GRP_ENTER * 8 + KEY_BIT_ENTER )

#define KEY_SNS_ESC    (1 << KEY_BIT_ESC)
#define KEY_SNS_CR     (1 << KEY_BIT_CR)
#define KEY_SNS_SPACE  (1 << KEY_BIT_SPACE)
#define KEY_SNS_LEFT   (1 << KEY_BIT_LEFT)
#define KEY_SNS_UP     (1 << KEY_BIT_UP)
#define KEY_SNS_RIGHT  (1 << KEY_BIT_RIGHT)
#define KEY_SNS_DOWN   (1 << KEY_BIT_DOWN)
#define KEY_SNS_ENTER  (1 << KEY_BIT_ENTER)

#define JOY_SNS_UP     (1 << 0)
#define JOY_SNS_DOWN   (1 << 1)
#define JOY_SNS_LEFT   (1 << 2)
#define JOY_SNS_RIGHT  (1 << 3)
#define JOY_SNS_A      (1 << 5)
#define JOY_SNS_B      (1 << 6)

#endif