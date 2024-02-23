declare module 'jwfsearch' {
  export function array(
    query: string,
    array: any[],
    fuzz: number
  ): Promise<{
    results: any[];
  }>;

  export function object(
    query: string,
    documents: any[],
    keys: string[],
    fuzz: number
  ): Promise<{
    results: any[];
  }>;
}
